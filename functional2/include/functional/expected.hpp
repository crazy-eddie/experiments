#ifndef UTILITY_EXPECTED_HPP
#define UTILITY_EXPECTED_HPP

#include <typeinfo>
#include <stdexcept>
#include <utility>

#include "monad.hpp"

namespace functional {

struct always_invalid
{
	always_invalid() = delete;
};

template < typename Result > struct expected;


template < >
struct expected<always_invalid>
{
	template < typename Exception >
	static expected from_exception(Exception const& exception)
	{
		if (typeid(exception) != typeid(Exception))
		{
			throw std::invalid_argument("Slicing detected.");
		}
		return from_exception(std::make_exception_ptr(exception));
	}

	static expected from_exception(std::exception_ptr p)
	{
		expected result;
		result.exception = std::exception_ptr(std::move(p));
		return result;
	}

	static expected from_exception()
	{
		return from_exception(std::current_exception());
	}

	std::exception_ptr error() const { return exception; }

private:
	expected() : exception() {}
	std::exception_ptr exception;
};

template < typename Result >
struct expected
{
	using value_type = Result;

	template < typename T
	         , typename E = typename std::enable_if<!std::is_same<T,expected<always_invalid>>::value>::type >
	expected(T && t)
		: result(t)
		, valid_result(true)
	{}
	expected(Result const& r)
		: result(r)
		, valid_result(true)
	{}
	expected(Result && r)
		: result(std::move(r))
		, valid_result(true)
	{}
	expected(expected const& other)
		: valid_result(other.valid_result)
	{
		if (valid_result) new (&result) Result(other.result);
		else new (&exception) std::exception_ptr(other.exception);
	}
	expected(expected && other)
		: valid_result(other.valid_result)
	{
		if (valid_result) new (&result) Result(std::move(other.result));
		else new (&exception) std::exception_ptr(std::move(other.exception));
	}
	expected(expected<always_invalid> const& other)
		: exception(other.error())
		, valid_result(false)
	{}

	~expected()
	{
		using std::exception_ptr;
		if (valid_result) result.~Result();
		else exception.~exception_ptr();
	}

	template < typename Exception >
	static expected from_exception(Exception const& exception)
	{
		if (typeid(exception) != typeid(Exception))
		{
			throw std::invalid_argument("Slicing detected.");
		}
		return from_exception(std::make_exception_ptr(exception));
	}

	static expected from_exception(std::exception_ptr p)
	{
		expected result;
		result.valid_result = false;
		new (&result.exception) std::exception_ptr(std::move(p));
		return result;
	}

	static expected from_exception()
	{
		return from_exception(std::current_exception());
	}

	void swap(expected & other)
	{
		if (valid_result)
		{
			if (other.valid_result)
			{
				using std::swap;
				swap(result, other.result);
			}
			else
			{
				auto tmp = std::move(other.exception);
				new (&other.result) Result(std::move(result));
				new (&exception) std::exception_ptr(tmp);
				std::swap(valid_result, other.valid_result);
			}
		}
		else
		{
			if (other.valid_result)
			{
				other.swap(*this);
			}
			else
			{
				exception.swap(other.exception);
				std::swap(valid_result, other.valid_result);
			}
		}
	}

	Result& get()
	{
		if (!valid_result) std::rethrow_exception(exception);
		return result;
	}
	Result const& get() const
	{
		if (!valid_result) std::rethrow_exception(exception);
		return result;
	}

	std::exception_ptr error() const
	{
		if (valid()) throw std::runtime_error("Attempt to retrieve error from expected that's valid.");
		return exception;
	}

	operator Result& () { return get(); }
	operator Result const& () const { return get(); }

	bool valid() const { return valid_result; }
private:
	union
	{
		Result result;
		std::exception_ptr exception;
	};
	bool valid_result;

	expected() {}
};



template < typename Exception >
expected<always_invalid> make_error(Exception && e)
{
	return expected<always_invalid>::from_exception(std::move(e));
}

template < typename Callable >
auto try_(Callable && callable)
{
	using ResultType = decltype(callable());

	try
	{
		return expected<ResultType>(callable());
	}
	catch (...)
	{
		return expected<ResultType>::from_exception(std::current_exception());
	}
}

struct Expected
{
	template < typename T >
	static auto construct(T && t)
	{
		return expected<typename std::remove_reference<T>::type>(std::forward<T>(t));
	}

	template < typename T, typename E >
	static auto error(E && e)
	{
		return expected<T>::from_exception(std::forward<E>(e));
	}

	template < typename Function
	         , typename Functor >
	static auto fmap(Function && function, Functor && functor)
	{
		// TODO: use reference_wrapper for reference returns
		using ResultType = typename std::remove_reference<decltype(function(functor.get()))>::type;
		if (!functor.valid()) return error<ResultType>(functor.error()); // TODO: forward

		return construct(function(functor.get()));
	}

	template < typename WrappedCallable
	         , typename WrappedArgument >
	static auto apply(WrappedCallable && callable, WrappedArgument && argument)
	{
		using ResultType = typename std::remove_reference<decltype(callable.get()(argument.get()))>::type;
		if (!callable.valid()) return error<ResultType>(callable.error());
		return fmap(callable.get(), argument);
	}

	template < typename Callable
	         , typename WrappedArgument >
	static auto bind(Callable && callable, WrappedArgument && argument)
	{
		// TODO: use reference_wrapper for reference returns
		using ResultType = typename std::remove_reference<decltype(callable(argument.get()))>::type;

		if (!argument.valid()) return ResultType::from_exception(argument.error()); // TODO: forward

		return callable(argument.get());
	}
};

template < typename T >
struct constructor<expected<T>>
{
	using type = Expected;
};


} // namespace functional

#endif
