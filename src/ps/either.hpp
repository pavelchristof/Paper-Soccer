#ifndef PS_EITHER_HPP
#define PS_EITHER_HPP

#include <utility>
#include <cassert>

namespace ps {

class TagLeft {};
class TagRight {};

/**
 * A variant type that contains either a left or a right value.
 */
template <typename Left, typename Right>
class Either
{
public:
	/**
	 * Initialize with a default left value.
	 * The default constructor is required by QVector.
	 */
	Either() {
		construct(Left{}, TagLeft{});
	}

	/**
	 * Constructs either a left or a right value.
	 * The tag can be used to resolve ambiguousness.
	 */
	// @{
	Either(const Left& left, TagLeft tag = {}) {
		construct(left, tag);
	}

	Either(const Right& right, TagRight tag = {}) {
		construct(right, tag);
	}

	Either(Left&& left, TagLeft tag = {}) {
		construct(left, tag);
	}

	Either(Right&& right, TagRight tag = {}) {
		construct(right, tag);
	}
	// @}

	/**
	 * Copies or moves an existing object.
	 */
	// @{
	Either(const Either& either) {
		construct(either);
	}

	Either(Either&& either) {
		construct(either);
	}
	// @}

	/**
	 * Calls the relevant destructor.
	 */
	~Either() {
		destruct();
	}

	/**
	 * Assignment.
	 */
	// @{
	Either& operator = (const Either& either) {
		destruct();
		construct(either);
		return *this;
	}

	Either& operator = (Either && either) {
		destruct();
		construct(either);
		return *this;
	}
	// @}

	/**
	 * Sets either a left or a right value. The old value is destroyed.
	 * The tag can be used to resolve ambiguousness.
	 */
	// @{
	void set(const Left& left, TagLeft tag = {}) {
		destruct();
		construct(left, tag);
	}

	void set(const Right& right, TagRight tag = {}) {
		destruct();
		construct(right, tag);
	}

	void set(Left&& left, TagLeft tag = {}) {
		destruct();
		construct(left, tag);
	}

	void set(Right&& right, TagRight tag = {}) {
		destruct();
		construct(right, tag);
	}
	// @}

	/**
	 * Accesses either the left or the right value.
	 */
	// @{
	Left& left() {
		assert(isLeft());
		return left_;
	}

	const Left& left() const {
		assert(isLeft());
		return left_;
	}

	Right& right() {
		assert(isRight());
		return right_;
	}

	const Right& right() const {
		assert(isRight());
		return right_;
	}
	// @}

	/**
	 * Is this a left value?
	 */
	bool isLeft() const {
		return isLeft_;
	}

	/**
	 * Is this a right value?
	 */
	bool isRight() const {
		return !isLeft_;
	}

private:
	void construct(const Left& left, TagLeft) {
		new(&left_) Left(left);
		isLeft_ = true;
	}

	void construct(const Right& right, TagRight) {
		new(&right_) Right(right);
		isLeft_ = false;
	}

	void construct(Left&& left, TagLeft) {
		new(&left_) Left(left);
		isLeft_ = true;
	}

	void construct(Right&& right, TagRight) {
		new(&right_) Right(right);
		isLeft_ = false;
	}

	void construct(const Either& either) {
		if (either.isLeft_) {
			construct(either.left_, TagLeft {});
		} else {
			construct(either.right_, TagRight {});
		}
		isLeft_ = either.isLeft_;
	}

	void construct(Either&& either) {
		if (either.isLeft_) {
			construct(std::move(either.left_), TagLeft {});
		} else {
			construct(std::move(either.right_), TagRight {});
		}
		isLeft_ = either.isLeft_;
	}

	void destruct() {
		if (isLeft_) {
			left_.~Left();
		} else {
			right_.~Right();
		}
	}

	bool isLeft_;
	union {
		Left left_;
		Right right_;
	};
};

} // namespace ps

#endif // PS_EITHER_HPP
