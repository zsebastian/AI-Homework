#include "SOM.h"

typedef Node<int, double, 3U> node_t;

SOM::SOM(size_t w, size_t h)
	:w_(w)
	,h_(h)
	,nodes_(w *h)
{

}

double SOM::euclidean_weight_distance(const node_t& n0, const node_t& n1) const
{
	double sum = 0.f;
	for (int i = 0; i < n0.dimensionality(); ++i)
	{
		sum += std::pow((n0[i] - n1[i]), 2.0f);
	}
	return std::sqrt(sum);
}

double SOM::euclidean_position_distance(const node_t& n0, const node_t& n1) const
{
	double sum = std::pow(n0.x() - n1.x(), 2.0f) + std::pow(n0.y() - n1.y(), 2.0f);
	return std::sqrt(sum);
}

double SOM::euclidean_color_distance(const node_t& n, const Color& c) const
{
	double sum = std::pow(n[0] - c.get_red(), 2.0f);
	sum += std::pow(n[1] - c.get_green(), 2.0f);
	sum += std::pow(n[2] - c.get_blue(), 2.0f);
	return std::sqrt(sum);
}

Color SOM::to_color(const node_t& n) const
{
	return Color::make_from_floats(n[0], n[1], n[2]);
}