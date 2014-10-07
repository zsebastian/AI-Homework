#pragma once

#include "Node.h"
#include <vector>
#include "Color.h"

class SOM
{
public:
	typedef Node<double, double, 3U> node_t;
	SOM(size_t w, size_t h);

	inline node_t operator()(int x, int y) { ensure_range(x, y); nodes_[y * static_cast<int>(w_) + x]; }
	inline size_t width() const { return w_; }
	inline size_t height() const{ return h_; }

	inline std::vector<node_t>::iterator begin(){ return nodes_.begin(); }
	inline std::vector<node_t>::iterator end(){ return nodes_.end(); }
	const inline std::vector<node_t>::const_iterator begin() const { return nodes_.begin(); }
	const inline std::vector<node_t>::const_iterator end() const { return nodes_.end(); }
	const inline std::vector<node_t>::const_iterator cbegin() const { return nodes_.cbegin(); }
	const inline std::vector<node_t>::const_iterator cend() const { return nodes_.cend(); }

	double euclidean_weight_distance(const node_t& n0, const node_t& n1) const;
	double euclidean_position_distance(const node_t& n0, const node_t& n1) const;
	double euclidean_color_distance(const node_t& n, const Color& c) const;
	Color to_color(const node_t& n) const;
private:
	inline bool ensure_range(int x, int y) const { return x >= 0 && x < w_ && y >= 0 && y < h_; };
	
	size_t w_, h_;
	std::vector<node_t> nodes_;
};