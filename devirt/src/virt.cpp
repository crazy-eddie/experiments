#include "../inc/virt.hpp"

#include <numeric>

struct base0 { virtual ~base0() {} };
struct base1 { virtual ~base1() {} };
struct base2 { virtual ~base2() {} };


#define BASE_SWARM base_virt//, base0, base1, base2

struct int_virt : BASE_SWARM
{
    int_virt(int i) : value(i) {}

    int get_int() const;

    double get_double() const;

private:

    int arr[10];

    int value;
};

struct double_virt : BASE_SWARM
{
    double_virt(double d) : value(d) {}

    int get_int() const;
    double get_double() const;

private:
    int arr[10];

    double value;
};



int int_virt::get_int() const { return value; }
double int_virt::get_double() const { return value; }


int double_virt::get_int() const
{
    return static_cast<int>(value);
}

double double_virt::get_double() const
{
    return value;
}


int get_int(base_virt const& bv)
{
    return bv.get_int();
}

double get_double(base_virt const& bv)
{
    return bv.get_double();
}


std::vector< std::shared_ptr<base_virt> >
generate_data_virt(int num)
{
    std::vector<std::shared_ptr<base_virt>> result;

    for (int i = 0; i < num; ++i)
    {
        if (i % 2) result.push_back(std::make_shared<int_virt>(i));
        else result.push_back(std::make_shared<double_virt>(i / 17.));
    }

    return result;
}

long long process_data( std::vector<std::shared_ptr<base_virt>> const& data
		              , int loop_num )
{
	long long result = 0;

	auto summer
	    = [](long long sum, std::shared_ptr<base_virt> const& val) -> long long
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
