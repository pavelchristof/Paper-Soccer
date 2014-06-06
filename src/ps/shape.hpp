#ifndef PS_SHAPE_HPP
#define PS_SHAPE_HPP

#include <tuple>
#include <cassert>

namespace ps {
namespace detail {

/**
 * Changes a list of types to a tuple of pairs of these types.
 */
template <typename... Ts>
struct TupleOfPairs;

template <typename T>
struct TupleOfPairs<T> {
	typedef std::tuple<std::pair<T, T>> Type;
};

template <typename T, typename... Ts>
struct TupleOfPairs<T, Ts...> {
	typedef std::tuple<std::pair<T, T>> ThisTuple;
	typedef typename TupleOfPairs<Ts...>::Type RestTuple;

	// Get the result type by concatenating the two tuples.
	typedef decltype(std::tuple_cat(
		std::declval<ThisTuple>(), 
		std::declval<RestTuple>()
	)) Type;
};

template <typename Shape, size_t keyIndex>
struct MapImpl {
	static size_t exec(const typename Shape::IntervalsTuple& intervals, const typename Shape::KeysTuple& keys, 
		size_t prefixCount, size_t prefixIndex)
	{
		const auto &range = std::get<keyIndex - 1>(intervals);
		const auto &key = std::get<keyIndex - 1>(keys);

		size_t layerCount = range.second - range.first + 1;
		size_t layerIndex = key - range.first;

		assert(layerCount >= 0);
		assert(layerIndex < layerCount);

		return MapImpl<Shape, keyIndex - 1>::exec(
			intervals,
			keys,
			prefixCount * layerCount,
			prefixCount * layerIndex + prefixIndex
		);
	}
};

template <typename Shape>
struct MapImpl<Shape, 0> {
	static size_t exec(const typename Shape::IntervalsTuple& intervals, const typename Shape::KeysTuple& keys, 
		size_t prefixCount, size_t prefixIndex)
	{
		return prefixIndex;
	}
};

template <typename Shape, size_t keyIndex>
struct SizeImpl {
	static size_t exec(const typename Shape::IntervalsTuple& intervals, size_t sizeSoFar)
	{
		const auto& range = std::get<keyIndex - 1>(intervals);

		size_t rangeSize = range.second - range.first + 1;
		assert(rangeSize >= 0);

		return SizeImpl<Shape, keyIndex - 1>::exec(
			intervals, rangeSize * sizeSoFar);
	}
};

template <typename Shape>
struct SizeImpl<Shape, 0> {
	static size_t exec(const typename Shape::IntervalsTuple& intervals, size_t sizeSoFar)
	{
		return sizeSoFar;
	}
};

} // namespace detail

/**
 * A mapping from a multidimensional box (key space) to an array index.
 * 
 * A key type must implement substraction with a result type convertible to size_t.
 * 
 * @param Keys key types
 */
template <typename... Keys>
class Shape
{
public:
	/**
	 * A tuple of keys.
	 */
	typedef std::tuple<Keys...> KeysTuple;

	/**
	 * A tuple of pairs of keys, representing intervals.
	 */
	typedef typename detail::TupleOfPairs<Keys...>::Type IntervalsTuple;

	/**
	 * The number of dimensions, equal to the number of keys.
	 */
	static const size_t dimensions = std::tuple_size<KeysTuple>::value;

	/**
	 * Constructs a from the given key intervals.
	 * @note Intervals are closed.
	 */
	Shape(const IntervalsTuple& intervals)
		: intervals_(intervals)
	{
	}

	/**
	 * @returns the key intervals.
	 */
	const IntervalsTuple& intervals() const
	{
		return intervals_;
	}

	/**
	 * Maps a point in the box to an index.
	 */
	size_t map(const KeysTuple& keys) const
	{
		return detail::MapImpl<Shape, dimensions>::exec(intervals_, keys, 1, 0);
	}

	/**
	 * The volume of the box (total number of keys).
	 */
	size_t size() const
	{
		return detail::SizeImpl<Shape, dimensions>::exec(intervals_, 1);
	}

private:
	IntervalsTuple intervals_;
};

} // namespace ps

#endif // PS_SHAPE_HPP
