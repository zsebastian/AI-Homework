#pragma once

#include <array>
#include <unordered_set>

template <class position_t, class weight_t, size_t weights_dimensionality_>
class Node
{
public:
	typedef std::unordered_set<Node<position_t, weight_t, weights_dimensionality_>*> set_t;

	Node(){}

	template <class InputIterator>
	Node(InputIterator iter_begin, InputIterator iter_end);

	inline const int dimensionality() const { return weights_dimensionality_; }
	inline weight_t& operator[](size_t index) { return data_[index]; };
	inline const weight_t& operator[](size_t index) const { return data_[index]; };

	inline position_t& x() { return position_[0]; }
	inline const position_t x() const { return position_[0]; }

	inline position_t& y() { return position_[1]; }
	inline const position_t y() const { return position_[1]; }

private:
	std::array<weight_t, weights_dimensionality_> data_;
	std::array<position_t, 2U> position_;
};


template <class position_t, class weight_t, size_t weights_dimensionality_>
template <class InputIterator>
Node<position_t, weight_t, weights_dimensionality_>::Node(InputIterator iter_begin, InputIterator iter_end)
{
	set_t::iterator it = neighbours_.begin();
	for (size_t i = 0; i < weights_dimensionality_; ++i)
	{
		vals_[i] = *iter_begin;
		iter_begin++;
	}
}