/*
 * Summer - summer/data/traits.h
 * author: Massimo Bianchi 2014
 */

#ifndef TRAITS_H_
#define TRAITS_H_

#include <type_traits>

namespace summer { namespace data {

/// SFINAE support: indicate a failure in substitution.
struct substitution_failure {};
/// SFINAE support: indicate if a it is a good substitution or not.
template<typename T> struct substitution_success : std::true_type {};
template<> struct substitution_success<substitution_failure> : std::false_type {};

// Alias for common type traits meta-function

template<bool cond, typename dest = void>
using enable_if = typename std::enable_if<cond, dest>::type;

template<typename dest>
using add_reference = typename std::add_lvalue_reference<dest>::type;

template<typename dest>
using add_const = typename std::add_const<dest>::type;

template<typename dest>
using remove_reference = typename std::remove_reference<dest>::type;

template<typename dest>
using remove_cv = typename std::remove_cv<dest>::type;

template<typename dest>
using is_copy_constructible = typename std::is_copy_constructible<dest>::type;

// checks...
static_assert(
	substitution_success<void>::value,
	"substitution_success<void> must by true."
);

static_assert(
	!substitution_success<substitution_failure>::value,
	"substitution_success<substitution_failure> must by false."
);

}}

#endif /* TRAITS_H_ */
