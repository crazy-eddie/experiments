#include "../inc/novirt.hpp"

#include <numeric>

int get_int_int(base_novirt const& bv)
{
    return static_cast<int_novirt const&>(bv).get_int();
}

int get_int_double(base_novirt const& bv)
{
    return static_cast<int>(
                static_cast<double_novirt const&>(bv).get_double());
}

int get_int(base_novirt const& bv)
{
    typedef int (*getter)(base_novirt const&);
    static getter getters[] =
    {
      get_int_int
    , get_int_double
    };

    return getters[bv.tag()](bv);
}


double get_double_int(base_novirt const& bv)
{
    return static_cast<int_novirt const&>(bv).get_int();
}

double get_double_double(base_novirt const& bv)
{
    return static_cast<double_novirt const&>(bv).get_double();
}

double get_double(base_novirt const& bv)
{
    typedef double (*getter)(base_novirt const&);
    static getter getters[] =
    {
      get_double_int
    , get_double_double
    };

    return getters[bv.tag()](bv);
}


std::vector< std::shared_ptr<base_novirt> >
generate_data_novirt(int num)
{
    std::vector<std::shared_ptr<base_novirt>> result;

    for (int i = 0; i < num; ++i)
    {
        if (i % 2)
            result.push_back(std::make_shared<int_novirt>(i));
        else
            result.push_back(std::make_shared<double_novirt>(i / 17.));
    }

    return result;
}


long long process_data( std::vector<std::shared_ptr<base_novirt>> const& data
		              , int loop_num )
{
	long long result = 0;

	auto summer
	    = [](long long sum, std::shared_ptr<base_novirt> const& val) -> long long
		  {
		      return sum + get_int(*val);
		  };

	for (int i = 0; i < loop_num; ++i)
		result = std::accumulate( std::begin(data)
	                            , std::end(data)
	                            , 0LL
	                            , summer );

	return result;
}
