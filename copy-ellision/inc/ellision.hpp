#ifndef ELLISION
#define ELLISION

#include <vector>

std::vector<int> generate_vector(int num);
void generate_vector(std::vector<int> & vec, int num);

void sort_vector_ref(std::vector<int> & vec);
std::vector<int> sort_vector_val(std::vector<int> vec);

template < typename Iter >
bool sorted(Iter begin, Iter end)
{
	if (std::distance(begin,end) < 2)
		return true;

	Iter next = begin; ++next;
	for (; next != end; ++begin, ++next)
		if (*begin > *next)
			return false;

	return true;
}

template < typename Range >
bool sorted(Range range)
{
	return sorted(std::begin(range), std::end(range));
}

#endif
