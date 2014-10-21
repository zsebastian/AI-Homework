#include "Solver.h"
#include "EvaluationFunctions.h"

Solver::Solver(std::vector<std::pair<evaluation_function, double>> evaluations)
	:current_piece_count_(-1)
	, level_(0)
	, score_(0.f)
	, running_(true)
	, lines_cleared_this_level_(0)
{
	evaluations_ = std::move(evaluations);
}

Solver::Solver()
	:current_piece_count_(-1)
	, level_(0)
	, score_(0.f)
	, running_(true)
	, lines_cleared_this_level_(0)
{
	evaluations_.emplace_back(EvaluationFunction<0>(), 1.0f);
	evaluations_.emplace_back(EvaluationFunction<1>(), 1.0f);
	evaluations_.emplace_back(EvaluationFunction<2>(), 1.0f);
	evaluations_.emplace_back(EvaluationFunction<3>(), 1.0f);
	evaluations_.emplace_back(EvaluationFunction<4>(), 1.0f);
	evaluations_.emplace_back(EvaluationFunction<5>(), 1.0f);
	evaluations_.emplace_back(EvaluationFunction<6>(), 1.0f);
}

void Solver::update(Board& board)
{
	if (running_ == false)
	{
		return;
	}

	if (board.get_piece_count() != current_piece_count_)
	{
		auto& current_piece = board.get_current_piece();
		if (!board.test_collision(current_piece))
		{
			current_piece_count_ = board.get_piece_count();
			start_search(board);
		}
	}

	play_recorded_actions(board);
}

bool Solver::is_done() const
{
	return !running_;
}

double Solver::get_score() const
{
	return score_;
}

void Solver::play_recorded_actions(Board& board)
{
	if (action_recording_.size() == 0)
	{
		handle_tick(board.tick());
		return;
	}
	else
	{
		auto& top_frame = action_recording_.front();
		if (top_frame.size() == 0)
		{
			handle_tick(board.tick());
			action_recording_.pop_front();
		}
		else
		{
			board.perform_action(top_frame.front());
			top_frame.pop_front();
		}
	}
}

void Solver::start_search(Board& board)
{
	PlayField play_field = board.create_play_field();
	
	std::vector<Piece> piece_queue;
	piece_queue.push_back(board.get_current_piece());
	for (int i = 0; i < 1; ++i)
	{
		piece_queue.push_back(board.get_next_piece(i));
	}
	StateArray states = build_states(play_field, piece_queue);
	auto& start_piece = board.get_current_piece();
	auto start = states[start_piece.get_x()][start_piece.get_y()][start_piece.get_rotation()][0];
	auto best = search(states, play_field, play_field, 0, piece_queue, std::vector<Piece>());
	action_recording_ = std::move(make_recording(best, start));
}

State::ptr Solver::search(StateArray& states, PlayField& original_play_field, PlayField& play_field, int depth, const std::vector<Piece>& piece_queue, std::vector<Piece>& locked_pieces)
{
	if (depth == piece_queue.size())
	{
		return nullptr;
	}

	StateQueue queue;
	const Piece& current = piece_queue[depth];

	queue.enqueue(states[current.get_x()][current.get_y()][current.get_rotation()][depth]);
	State::ptr best_state;
	double best_state_value;

	while (!queue.is_empty())
	{
		auto state = queue.dequeue();
		const Piece& current = state->piece;

		Piece move_left = current;
		Piece move_right = current;
		Piece rotate_right = current;
		Piece move_down = current;
		move_left.move(-1, 0);
		move_right.move(1, 0);

		rotate_right.rotate_right();
		move_down.move(0, 1);

		if (current.get_max_rotations() != 1)
		{
			add_state_to_queue(states, queue, state, play_field, rotate_right, depth);
		}
		add_state_to_queue(states, queue, state, play_field, move_left, depth);
		add_state_to_queue(states, queue, state, play_field, move_right, depth);
		
		if (!add_state_to_queue(states, queue, state, play_field, move_down, depth))
		{
			PlayField next_play_field = play_field;
			next_play_field.imprint(current);
			locked_pieces.push_back(current);
			State::ptr next_search = search(states, original_play_field, next_play_field, depth + 1, piece_queue, locked_pieces);

			if (next_search == nullptr)
			{
				
				double eval = evaluate_play_field(original_play_field, next_play_field, locked_pieces);
				if (best_state == nullptr || best_state_value > eval)
				{
					best_state = states[current.get_x()][current.get_y()][current.get_rotation()][depth];
					best_state_value = eval;
				}
			}
			locked_pieces.pop_back();
		}
	}
	return best_state;
}

Solver::StateArray Solver::build_states(const PlayField& play_field, const std::vector<Piece>& piece_queue) const
{
	int w = play_field.get_width();
	int h = play_field.get_height();
	int d = 4;
	auto ret = StateArray(w, h, d, piece_queue.size());
	
	for (int z = 0; z < d; ++z)
	{
		for (int y = 0; y < h; ++y)
		{
			for (int x = 0; x < w; ++x)
			{
				for (int s = 0; s < piece_queue.size(); ++s)
				{
					ret[x][y][z][s] = std::make_shared<State>();
					ret[x][y][z][s]->visited = false;
					ret[x][y][z][s]->piece = piece_queue[s];
					ret[x][y][z][s]->piece.set(x, y, z);
				}
			}
		}
	}
	return std::move(ret);
}

bool Solver::add_state_to_queue(StateArray& states, StateQueue& queue, State::ptr prev_state, const PlayField& play_field, const Piece& piece, int depth)
{
	int x = piece.get_x();
	int y = piece.get_y();
	int z = piece.get_rotation();

	if (play_field.test_collision(piece))
	{
		return false;
	}

	auto& state = states[x][y][z][depth];
	if (x == prev_state->piece.get_x() &&
		y == prev_state->piece.get_y() &&
		z == prev_state->piece.get_rotation())
	{
		return true;
	}
	if (state->visited)
	{
		return true;
	}

	state->visited = true;
	state->predecessor = prev_state;
	queue.enqueue(state);
	return true;
}

double Solver::evaluate_play_field(const PlayField& from, const PlayField& to, const std::vector<Piece>& piece_queue) const
{
	double total = 0;
	for (auto& p: evaluations_)
	{
		total += p.first(from, to, piece_queue) * p.second;
	}
	return total;
}

void Solver::handle_tick(int tick)
{
	if (tick == -1)
	{
		running_ = false;
	}

	double score_multiplier = 0.f;
	if (tick == 1)
	{
		score_multiplier = 0.1f;
	}
	else if (tick == 2)
	{
		score_multiplier = 0.3f;
	}
	else if (tick == 3)
	{
		score_multiplier = 0.6f;
	}
	else if (tick >= 4)
	{
		score_multiplier = 1.0f;
	}
	lines_cleared_this_level_ += tick;
	if (lines_cleared_this_level_ >= 10)
	{
		level_++;
		lines_cleared_this_level_ -= 10;
	}
	score_ += score_multiplier * (level_ + 1);
}

Solver::Recording Solver::make_recording(State::ptr state, State::ptr start) const
{
	Recording ret;
	
	State::ptr current = state;
	while (current != nullptr && !current->predecessor.expired() && start != current)
	{
		ret.emplace_front();
		State::ptr prev = current->predecessor.lock();
		auto current_piece = current->piece;
		auto prev_piece = prev->piece;

			//get in rotation
		while (current_piece.get_rotation() != prev_piece.get_rotation())
		{
			current_piece.rotate_left();
			ret.front().emplace_front(Board::Action::Rotate);
		}

		//move to the right x
		while (current_piece.get_x() < prev_piece.get_x())
		{
			current_piece.move(1, 0);
			ret.front().emplace_front(Board::Action::Left);
		}
		while (current_piece.get_x() > prev_piece.get_x())
		{
			current_piece.move(-1, 0);
			ret.front().emplace_front(Board::Action::Right);
		}

		current = prev;
	}

	return std::move(ret);
}
