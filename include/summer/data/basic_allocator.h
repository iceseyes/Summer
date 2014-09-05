/*
 * Summer - summer/data/basic_allocator.h
 * author: Massimo Bianchi 2014
 */

#ifndef BASIC_ALLOCATOR_H_
#define BASIC_ALLOCATOR_H_

namespace summer { namespace data {

/// Create a new instance for a type T
template<class T> struct basic_allocator {
	typedef T target_type;			//!< type of created object

	/// Return a new instance of target_type
	/// @return a new instance of target_type
	target_type *_create() const { return new target_type(); }
};

}}



#endif /* BASIC_ALLOCATOR_H_ */
