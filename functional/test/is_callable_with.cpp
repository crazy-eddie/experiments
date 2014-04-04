#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Callable with
#include <boost/test/unit_test.hpp>

#include "../inc/core/util/meta/is_callable_with.hpp"

namespace meta = functional::core::util::meta;

void fun(int,double) {}

BOOST_AUTO_TEST_CASE(functions)
{
	using fun_t = decltype(fun);

	BOOST_CHECK((meta::is_callable_with<fun_t,int,double>::value));
	BOOST_CHECK((meta::is_callable_with<fun_t,int,double>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::true_, meta::is_callable_with<fun_t,int,double>::type>::value));

	BOOST_CHECK((!meta::is_callable_with<fun_t,int>::value));
	BOOST_CHECK((!meta::is_callable_with<fun_t,int>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::false_, meta::is_callable_with<fun_t,int>::type>::value));

	BOOST_CHECK((meta::is_callable_with<fun_t,int,int>::value));
	BOOST_CHECK((meta::is_callable_with<fun_t,int,int>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::true_, meta::is_callable_with<fun_t,int,int>::type>::value));

	BOOST_CHECK((!meta::is_callable_with<fun_t,int,double*>::value));
	BOOST_CHECK((!meta::is_callable_with<fun_t,int,double*>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::false_, meta::is_callable_with<fun_t,int,double*>::type>::value));
}

BOOST_AUTO_TEST_CASE(lambda)
{
	auto fun = [](int,double) {};
	using fun_t = decltype(fun);

	BOOST_CHECK((meta::is_callable_with<fun_t,int,double>::value));
	BOOST_CHECK((meta::is_callable_with<fun_t,int,double>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::true_, meta::is_callable_with<fun_t,int,double>::type>::value));

	BOOST_CHECK((!meta::is_callable_with<fun_t,int>::value));
	BOOST_CHECK((!meta::is_callable_with<fun_t,int>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::false_, meta::is_callable_with<fun_t,int>::type>::value));

	BOOST_CHECK((meta::is_callable_with<fun_t,int,int>::value));
	BOOST_CHECK((meta::is_callable_with<fun_t,int,int>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::true_, meta::is_callable_with<fun_t,int,int>::type>::value));

	BOOST_CHECK((!meta::is_callable_with<fun_t,int,double*>::value));
	BOOST_CHECK((!meta::is_callable_with<fun_t,int,double*>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::false_, meta::is_callable_with<fun_t,int,double*>::type>::value));
}

BOOST_AUTO_TEST_CASE(auto_lambda)
{
	auto fun = [](auto,auto) {};
	using fun_t = decltype(fun);

	BOOST_CHECK((meta::is_callable_with<fun_t,int,double>::value));
	BOOST_CHECK((meta::is_callable_with<fun_t,int,double>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::true_, meta::is_callable_with<fun_t,int,double>::type>::value));

	BOOST_CHECK((!meta::is_callable_with<fun_t,int>::value));
	BOOST_CHECK((!meta::is_callable_with<fun_t,int>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::false_, meta::is_callable_with<fun_t,int>::type>::value));

	BOOST_CHECK((meta::is_callable_with<fun_t,int,int>::value));
	BOOST_CHECK((meta::is_callable_with<fun_t,int,int>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::true_, meta::is_callable_with<fun_t,int,int>::type>::value));

	BOOST_CHECK((meta::is_callable_with<fun_t,int,double*>::value));
	BOOST_CHECK((meta::is_callable_with<fun_t,int,double*>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::true_, meta::is_callable_with<fun_t,int,double*>::type>::value));
}

BOOST_AUTO_TEST_CASE(binds)
{
	using namespace std::placeholders;
	using fun_t = decltype(std::bind(fun, _1, _2));

	BOOST_CHECK((meta::is_callable_with<fun_t,int,double>::value));
	BOOST_CHECK((meta::is_callable_with<fun_t,int,double>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::true_, meta::is_callable_with<fun_t,int,double>::type>::value));

	BOOST_CHECK((!meta::is_callable_with<fun_t,int>::value));
	BOOST_CHECK((!meta::is_callable_with<fun_t,int>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::false_, meta::is_callable_with<fun_t,int>::type>::value));

	BOOST_CHECK((meta::is_callable_with<fun_t,int,int>::value));
	BOOST_CHECK((meta::is_callable_with<fun_t,int,int>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::true_, meta::is_callable_with<fun_t,int,int>::type>::value));

	BOOST_CHECK((!meta::is_callable_with<fun_t,int,double*>::value));
	BOOST_CHECK((!meta::is_callable_with<fun_t,int,double*>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::false_, meta::is_callable_with<fun_t,int,double*>::type>::value));
}

BOOST_AUTO_TEST_CASE(std_function)
{
	using fun_t = std::function<void(int,double)>;

	BOOST_CHECK((meta::is_callable_with<fun_t,int,double>::value));
	BOOST_CHECK((meta::is_callable_with<fun_t,int,double>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::true_, meta::is_callable_with<fun_t,int,double>::type>::value));

	BOOST_CHECK((!meta::is_callable_with<fun_t,int>::value));
	BOOST_CHECK((!meta::is_callable_with<fun_t,int>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::false_, meta::is_callable_with<fun_t,int>::type>::value));

	BOOST_CHECK((meta::is_callable_with<fun_t,int,int>::value));
	BOOST_CHECK((meta::is_callable_with<fun_t,int,int>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::true_, meta::is_callable_with<fun_t,int,int>::type>::value));

	BOOST_CHECK((!meta::is_callable_with<fun_t,int,double*>::value));
	BOOST_CHECK((!meta::is_callable_with<fun_t,int,double*>::type::value));
	BOOST_CHECK((boost::is_same<boost::mpl::false_, meta::is_callable_with<fun_t,int,double*>::type>::value));

}

