#ifndef VIRT
#define VIRT

#include <memory>
#include <vector>

struct base_virt
{
    virtual ~base_virt() {}

    virtual int get_int() const = 0;
    virtual double get_double() const = 0;
};

int get_int(base_virt const&);
double get_double(base_virt const&);

std::vector< std::shared_ptr<base_virt> >
generate_data_virt(int);


long long
process_data(std::vector< std::shared_ptr<base_virt> > const&, int);

#endif
