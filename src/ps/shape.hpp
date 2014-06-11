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
	static const size_t dimensions = sizeof...(Keys);

	/**
	 * Constructs a box from the given key intervals.
	 * @note Intervals are closed.
	 */
	Shape(const IntervalsTuple& intervals)
		: intervals_(intervals)
	{
	}

	Shape(const Shape&) = default;
	Shape(Shape&&) = default;
	Shape& operator =(const Shape&) = default;
	Shape& operator =(Shape&&) = default;

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
		return MapImpl<dimensions>::exec(intervals_, keys, 1, 0);
	}

	/**
	 * The volume of the box (total number of keys).
	 */
	size_t size() const
	{
		return SizeImpl<dimensions>::exec(intervals_, 1);
	}

private:
	IntervalsTuple intervals_;

	template <size_t dim, typename Dummy = void>
	struct MapImpl {
		static size_t exec(const IntervalsTuple& intervals, const KeysTuple& keys, 
						   size_t prefixCount, size_t prefixIndex)
		{
			const auto& range = std::get<dim - 1>(intervals);
			const auto& key = std::get<dim - 1>(keys);

			size_t layerCount = size_t(range.second - range.first) + 1;
			size_t layerIndex = size_t(key - range.first);

			assert(layerIndex < layerCount);

			return MapImpl<dim - 1>::exec(
				intervals,
				keys,
				prefixCount * layerCount,
				prefixCount * layerIndex + prefixIndex
			);
		}
	};

	template <typename Dummy>
	struct MapImpl<0, Dummy> {
		static size_t exec(const IntervalsTuple& intervals, const KeysTuple& keys, 
						   size_t prefixCount, size_t prefixIndex)
		{
			return prefixIndex;
		}
	};

	template <size_t dim, typename Dummy = void>
	struct SizeImpl {
		static size_t exec(const IntervalsTuple& intervals, size_t sizeSoFar)
		{
			const auto& range = std::get<dim - 1>(intervals);
			size_t rangeSize = size_t(range.second - range.first) + 1;
			return SizeImpl<dim - 1>::exec(intervals, rangeSize * sizeSoFar);
		}
	};

	template <typename Dummy>
	struct SizeImpl<0, Dummy> {
		static size_t exec(const IntervalsTuple& intervals, size_t sizeSoFar)
		{
			return sizeSoFar;
		}
	};
};

} // namespace ps

#endif // PS_SHAPE_HPP
