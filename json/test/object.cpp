#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE json-object
#include <boost/test/unit_test.hpp>

#include "../include/json.hpp"


BOOST_AUTO_TEST_CASE(key)
{
	auto k0 = JSON_KEY("key0");
	BOOST_CHECK_EQUAL(static_cast<char const*>(k0), "key0");

	auto k1 = JSON_KEY("key1");
	BOOST_CHECK_EQUAL(static_cast<char const*>(k1), "key1");

	BOOST_CHECK(!(std::is_same<decltype(k0), decltype(k1)>::value));
}

BOOST_AUTO_TEST_CASE(definition)
{
	auto obj0 = json::object(JSON_KEY("blob"));
	BOOST_CHECK_EQUAL(obj0.schema(), "blob");

	auto obj1 = json::object( JSON_KEY("blob")
							, json::field(JSON_KEY("field0"), json::boolean, "Test document string.")
							, json::field(JSON_KEY("field1"), json::string) );

	BOOST_CHECK((std::is_same< decltype(obj1.field_reader(JSON_KEY("field0")))
			                 , decltype(json::boolean)>::value));
	BOOST_CHECK_EQUAL(obj1.docstring(JSON_KEY("field0")), "Test document string.");

	using value_type = decltype(json::read(obj1.nonull(),""));
	BOOST_CHECK((std::is_same< decltype(std::declval<value_type>().get(JSON_KEY("field0")))
			                 , boost::optional<bool> >::value));
	BOOST_CHECK((std::is_same< decltype(std::declval<value_type>().set(JSON_KEY("field1"), "blah"))
			                 , void >::value));
}

BOOST_AUTO_TEST_CASE(object_values)
{
	auto field0 = JSON_KEY("field0");
	auto field1 = JSON_KEY("field1");
	auto obj1 = json::object( JSON_KEY("blob")
							, json::field(JSON_KEY("field0"), json::boolean)
							, json::field(JSON_KEY("field1"), json::string) );

	using value_type = std::remove_reference<decltype(json::read(obj1.nonull(), ""))>::type;

	value_type value;

	value.set(field0, true);
	BOOST_CHECK(value.get(field0));
	BOOST_CHECK(*value.get(field0));

	value.set(field1, "hello");
	BOOST_CHECK_EQUAL(*value.get(field1), "hello");
}

BOOST_AUTO_TEST_CASE(read_object)
{
	auto field0 = JSON_KEY("field0");
	auto field1 = JSON_KEY("field1");

	auto obj = json::object( JSON_KEY("blob")
			               , json::field(field0, json::boolean)
			               , json::field(field1, json::string)).nonull();

	auto val = json::read(obj, "{\"field1\":\"hello\", \"field0\": true}");

	BOOST_CHECK(val.get(field0));
	BOOST_CHECK_EQUAL(*val.get(field0), true);

	BOOST_CHECK(val.get(field1));
	BOOST_CHECK_EQUAL(*val.get(field1), "hello");
}


BOOST_AUTO_TEST_CASE(object_with_object_member)
{
	auto field0 = JSON_KEY("field0");

	auto obj = json::object( JSON_KEY("blob")
						   , json::field(field0, json::number.nonull(), "Test number") ).nonull();
	auto outer = json::object( JSON_KEY("outer")
			                 , json::field(field0, obj.nonull()) ).nonull();

	auto val = json::read(outer, "{\"field0\":{\"field0\":666}}");

	BOOST_CHECK_EQUAL(666, static_cast<int>(val.get(field0).get(field0)));
	BOOST_CHECK_EQUAL(outer.field_reader(field0).schema(), "blob");
}
