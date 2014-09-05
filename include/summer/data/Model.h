/*
 * Summer - summer/data/Model.h
 * author: Massimo Bianchi 2014
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <summer/data/traits.h>
#include <boost/any.hpp>

#include <map>
#include <string>

namespace summer { namespace data {

class Model :
	public std::map<std::string, boost::any> {
public:
	template<typename dest> dest &get(const key_type &key) {
		return boost::any_cast<dest &>(at(key));
	}
};

// Check for model injector.
template<typename T> class get_model_injector_result {
	template<typename C, typename M>
	static auto check(C controller, M m)
		-> decltype(controller.model(m));

	static substitution_failure check(...);

public:
	using type = decltype(check(T(), Model()));
};

// Check for model injector.
template<typename T> class get_model_direct_injector_result {
	template<typename C, typename M>
	static auto check(C controller, M model) -> decltype(controller(model));

	static substitution_failure check(...);

public:
	using type = decltype(check(T(), Model()));
};

template<typename T> struct Has_model_injector :
	substitution_success<typename get_model_injector_result<T>::type> {};

template<typename T> struct Has_model_direct_injector :
	substitution_success<typename get_model_direct_injector_result<T>::type> {};

template<typename T>
constexpr bool has_model_injector() { return Has_model_injector<T>::value; }

template<typename T>
constexpr bool has_model_direct_injector() { return Has_model_direct_injector<T>::value; }

}}

#endif /* MODEL_H_ */
