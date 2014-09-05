/*
 * Summerde/summer/data/serialize_policies.h
 * author: Massimo Bianchi 2014
 *
 * Policies for convert data in string and back.
 */

#ifndef SUMMER_DATA_SERIALIZE_POLICIES_H_
#define SUMMER_DATA_SERIALIZE_POLICIES_H_

#include <summer/data/traits.h>

#include <sstream>
#include <string>

namespace summer { namespace data {

/// Convert User data to std::string and back through OStream interface
template<typename T> struct OStreamSerializePolicy {
	using type 			= T;
	using typeRef 		= data::add_reference<type>;
	using typeConstRef 	= data::add_reference<data::add_const<type>>;

	std::string toString(typeConstRef data) const {
		std::stringstream ss;
		ss << data;
		return ss.str();
	}

	void fromString(typeRef out, const std::string &data) const {
		std::stringstream ss {data};
		ss >> out;
	}
};

}}

#endif /* SUMMER_DATA_SERIALIZE_POLICIES_H_ */
