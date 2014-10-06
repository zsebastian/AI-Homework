#pragma once

#include <array>

template <class T, size_t weights_dimensionality_>
class Node
{
public:
	template <class InputIterator>
	Node(InputIterator iter_begin, InputIterator iter_end);

	T& operator[](size_t index);
	const T& operator[](size_t index) const;

private:

	std::array<T, weights_dimensionality_> weights_;
};


template <class T, size_t weights_dimensionality_>
template <class InputIterator>
Node<T, weights_dimensionality_>::Node(InputIterator iter_begin, InputIterator iter_end)
{
	for (size_t i = 0; i < weights_dimensionality_; ++i)
	{
		vals_[i] = *iter_begin;
		iter_begin++;
	}
}

template <class T, size_t weights_dimensionality_>
T& Node<T, weights_dimensionality_>::operator[](size_t index)
{
	return vals_[i];
}


template <class T, size_t weights_dimensionality_>
const T& Node<T, weights_dimensionality_>::operator[](size_t index) const
{
	return vals_[i];
}
