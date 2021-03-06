/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_DESERIALIZER_IMPL_H
#define SERIALIZEPP_DESERIALIZER_IMPL_H

#include "concepts.h"
#include "value_identity.h"

namespace spp::detail {

template<typename T, typename D>
constexpr T call_deserializer(D& serializer);

}

#include "deserializer_impl_container.h"
#include "deserializer_impl_numeric.h"
#include "deserializer_impl_algebraic.h"
#include "deserializer_impl_enum.h"

namespace spp::detail {

template<typename T>
void not_deserializable_error() {
	static_assert(indirect_false_v<T>, R"(
Type is not deserializable!

For user-defined types to be deserializable, add a public constructor

	template<std::derived_from<spp::deserializer_base> Deserializer>
	explicit constexpr /*Type*/(Deserializer& d) {
		// ...
	}

or a static method

	template<std::derived_from<spp::deserializer_base> Deserializer>
	constexpr static /*Type*/ deserialize(Deserializer& d) {
		// ...
	}

to the type.

)");
}

template<typename T, typename D>
constexpr T call_deserializer(D& deserializer) {
	using Tp = std::remove_cvref_t<T>;

	if constexpr (builtin_deserializable<Tp, D>) {
		return deserializer_impl<Tp>{}(deserializer);
	} else if constexpr (user_defined_deserializable_constructor<Tp, D>) {
		return T(deserializer);
	} else if constexpr (user_defined_deserializable_method<Tp, D>) {
		return T::deserialize(deserializer);
	} else {
		not_deserializable_error<Tp>();
	}
}

}

#endif
