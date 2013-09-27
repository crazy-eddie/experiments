#ifndef NOVIRT
#define NOVIRT

#include <memory>
#include <vector>

struct base_novirt
{
    base_novirt(int i) : index(i) {}

    int tag() const { return index; }

private:

    int index;
};

struct int_novirt : base_novirt
{
    int_novirt(int i)
        : base_novirt(0)
        , value(i)
    {}

    int get_int() const { return value; }

private:

    int value;
};

struct double_novirt : base_novirt
{
    double_novirt(double d)
        : base_novirt(1)
        , value(d)
    {}

    double get_double() const { return value; }

private:

    double value;
};


int get_int(base_novirt const&);
double get_double(base_novirt const&);


std::vector< std::shared_ptr<base_novirt> >
generate_data_novirt(int);

long long process_data(std::vector<std::shared_ptr<base_novirt>> const&, int);

#endif
