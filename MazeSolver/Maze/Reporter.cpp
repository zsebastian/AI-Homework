
class WallAhead : public Reporter
{
	bool evaluate(Maze& maze, Coord current_coord) override;
};

class WallLeft : public Reporter
{
	bool evaluate(Maze& maze, Coord current_coord) override;
};

class WallRight : public Reporter
{
	bool evaluate(Maze& maze, Coord current_coord) override;
};
