#include "../inc/ellision.hpp"

#include <algorithm>

std::vector<int> generate_vector(int num)
{
	std::vector<int> result;

	while (num)
		result.push_back(num--);

	return result;
}

void generate_vector(std::vector<int> & vec, int num)
{
	while (num) vec.push_back(num--);
}


void sort_vector_ref(std::vector<int> & vec)
{
	std::sort(std::begin(vec), std::end(vec));
}

std::vector<int> sort_vector_val(std::vector<int> vec)
{
	std::vector<int> result;

	std::sort(std::begin(vec), std::end(vec));

	result.swap(vec);
	return result;
}

