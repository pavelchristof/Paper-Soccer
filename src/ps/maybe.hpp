#ifndef PS_MAYBE_HPP
#define PS_MAYBE_HPP

#include <cassert>
#include <utility>
#include <functional>
#include <type_traits>

namespace ps {

class None {};
const None none;

/**
 * An utility class that can either contain a value or be empty.
 */
template <typename T>
class Maybe
{
public:
	// Maybe<None> would cause ambiguous overloading and isn't usefull at all.
	static_assert(!std::is_same<T, None>::value, "Use a bool instead of Maybe<None>.");

	/**
	 * Creates an empty object.
	 */
	Maybe(None)
	{
		construct(none);
	}

	/**
	 * Creates an object containing some value.
	 */
	// @{
	Maybe(const T& some)
	{
		construct(some);
	}

	Maybe(T&& some)
	{
		construct(some);
	}
	// @}

	/**
	 * Copies or moves an existing object.
	 */
	// @{
	Maybe(const Maybe& maybe)
	{
		construct(maybe);
	}

	Maybe(Maybe&& maybe)
	{
		construct(maybe);
	}
	// @}

	/**
	 * Destroys the contained value if it exists.
	 */
	~Maybe() 
	{
		destruct();
	}

	/**
	 * Assignment.
	 */
	// @{
	Maybe& operator =(None)
	{
		destruct();
		construct(none);
		return *this;
	}

	Maybe& operator =(const T& some)
	{
		destruct();
		construct(some);
		return *this;
	}

	Maybe& operator =(T&& some)
	{
		destruct();
		construct(some);
		return *this;
	}

	Maybe& operator =(const Maybe& maybe)
	{
		destruct();
		construct(maybe);
		return *this;
	}

	Maybe& operator =(Maybe&& maybe)
	{
		destruct();
		construct(maybe);
		return *this;
	}
	// @}

	/**
	 * Equality comparison.
	 */
	// @{
	bool operator ==(const Maybe& maybe) const {
		if (isSome()) {
			return maybe.isSome() && get() == maybe.get();
		} else {
			return maybe.isNone();
		}
	}

	bool operator !=(const Maybe& maybe) const {
		if (isSome()) {
			return maybe.isNone() || get() != maybe.get();
		} else {
			return maybe.isSome();
		}
	}
	// @}

	/**
	 * Conversion to a const value.
	 */
	operator Maybe<const T>() const 
	{
		return map<const T>([] (const T& value) -> const T { return value; });
	}

	/**
	 * Conversion to a const reference.
	 */
	operator Maybe<const T&>() const
	{
		return map<const T&>([] (const T& value) -> const T& { return value; });
	}

	/**
	 * Whether the object contains a value.
	 */
	bool isSome() const
	{
		return isSome_;
	}

	/**
	 * Whether the object is empty.
	 */
	bool isNone() const
	{
		return !isSome_;
	}

	/**
	 * Get the contained value.
	 * @warning Call only if isSome().
	 */
	// @{
	T& get()
	{
		assert(isSome());
		return some_;
	}

	const T& get() const
	{
		assert(isSome());
		return some_;
	}
	// @}

	/**
	 * Maps an Maybe<T> to Maybe<R> by applying a function to the contained value.
	 */
	template <typename R>
	Maybe<R> map(std::function<R (const T&)> f) const
	{
		if (isSome()) {
			return f(get());
		} else {
			return none;
		}
	}

	/**
	 * Applies a function to the contained value or returns a default.
	 */
	template <typename R>
	R mapOr(std::function<R (const T&)> f, const R& def) const
	{
		if (isSome()) {
			return f(get());
		} else {
			return def;
		}
	}

	/**
	 * Maps an Maybe<T> to Maybe<R> by applying a function to the contained value.
	 */
	template <typename R>
	Maybe<R> bind(std::function<Maybe<R> (const T&)> f) const
	{
		if (isSome()) {
			return f(get());
		} else {
			return none;
		}
	}

private:
	void construct(const T& some) 
	{
		new (&some_) T(some);
		isSome_ = true;
	}

	void construct(T&& some)
	{
		new (&some_) T(some);
		isSome_ = true;
	}

	void construct(None) 
	{
		isSome_ = false;
	}

	void construct(const Maybe& maybe)
	{
		isSome_ = maybe.isSome_;
		if (isSome_) {
			new (&some_) T(maybe.some_);
		}
	}

	void construct(Maybe&& maybe)
	{
		isSome_ = maybe.isSome_;
		maybe.isSome_ = false;
		if (isSome_) {
			new (&some_) T(std::move(maybe.some_));
		}
	}

	void destruct() 
	{
		if (isSome_) {
			some_.~T();
		}
	}

	// A single element union prevents implicit construction.
	union {
		T some_;
	};
	bool isSome_;
};

/**
 * Maybe<T> specialized for reference types.
 */
template <typename T>
class Maybe<T&>
{
public:
	/**
	 * Creates an empty object.
	 */
	Maybe(None)
	{
		set(none);
	}

	/**
	 * Creates an object containing some value.
	 */
	Maybe(T& some)
	{
		set(some);
	}

	/**
	 * Copies an existing object.
	 */
	Maybe(const Maybe& maybe)
	{
		set(maybe);
	}

	/**
	 * Assignment.
	 */
	// @{
	Maybe& operator =(None)
	{
		set(none);
		return *this;
	}

	Maybe& operator =(T& some)
	{
		set(some);
		return *this;
	}

	Maybe& operator =(const Maybe& maybe)
	{
		set(maybe);
		return *this;
	}
	// @}
	
	/**
	 * Equality comparison.
	 */
	// @{
	bool operator ==(const Maybe& maybe) const {
		if (isSome()) {
			return maybe.isSome() && get() == maybe.get();
		} else {
			return maybe.isNone();
		}
	}

	bool operator !=(const Maybe& maybe) const {
		if (isSome()) {
			return maybe.isNone() || get() != maybe.get();
		} else {
			return maybe.isSome();
		}
	}
	// @}

	/**
	 * Conversion to Maybe<const T&>.
	 */
	operator Maybe<const T&>() const 
	{
		return map<const T&>([] (const T& value) -> const T& { return value; });
	}

	/**
	 * Conversion to a value using copy constructor.
	 */
	operator Maybe<typename std::remove_const<T>::type>() const
	{
		return map<typename std::remove_const<T>::type>([] (const T& value) -> T { return T{value}; });
	}

	/**
	 * Whether the object contains a value.
	 */
	bool isSome() const
	{
		return pointer != nullptr;
	}

	/**
	 * Whether the object is empty.
	 */
	bool isNone() const
	{
		return pointer == nullptr;
	}

	/**
	 * Get the contained value.
	 * @warning Call only if isSome().
	 */
	// @{
	T& get() const
	{
		assert(isSome());
		return *pointer;
	}
	// @}

	/**
	 * Maps an Maybe<T> to Maybe<R> by applying a function to the contained value.
	 */
	template <typename R>
	Maybe<R> map(std::function<R (T&)> f) const
	{
		if (isSome()) {
			return f(get());
		} else {
			return none;
		}
	}

	/**
	 * Applies a function to the contained value or returns a default.
	 */
	template <typename R>
	R mapOr(std::function<R (T&)> f, const R& def) const
	{
		if (isSome()) {
			return f(get());
		} else {
			return def;
		}
	}

	/**
	 * Maps an Maybe<T> to Maybe<R> by applying a function to the contained value.
	 */
	template <typename R>
	Maybe<R> bind(std::function<Maybe<R> (T&)> f) const
	{
		if (isSome()) {
			return f(get());
		} else {
			return none;
		}
	}

private:
	void set(T& some) 
	{
		pointer = &some;
	}

	void set(None) 
	{
		pointer = nullptr;
	}

	void set(const Maybe& maybe)
	{
		pointer = maybe.pointer;
	}

	// Represent the reference as a nullable pointer.
	T* pointer;
};

template <typename T>
Maybe<T> some(const T& value) 
{
	return Maybe<T>(value);
}

template <typename T>
Maybe<T> some(T&& value)
{
	return Maybe<T>(value);
}

} // namespace ps

#endif // PS_MAYBE_HPP
