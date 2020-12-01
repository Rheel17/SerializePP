/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_DESERIALIZER_IMPL_H
#define SERIALIZEPP_DESERIALIZER_IMPL_H

#include "concepts.h"
#include "value_identity.h"

namespace spp::detail {

template<typename T, typename D>
constexpr T call_deserializer(D& serializer) noexcept;

}

#include "deserializer_impl_container.h"
#include "deserializer_impl_numeric.h"
#include "deserializer_impl_algebraic.h"

namespace spp::detail {

template<typename T>
void not_deserializable_error() {
	static_assert(indirect_false_v<T>, R"(
Type is not deserializable!

For user-defined types to be deserializable, add a public constructor

	template<typename Deserializer>
	explicit /*Type*/(Deserializer& d) noexcept {
		// ...
	}

to the type.

)");
}

template<typename T, typename D>
constexpr T call_deserializer(D& deserializer) noexcept {
	using Tp = std::remove_cvref_t<T>;

	if constexpr (builtin_deserializable<Tp, D>) {
		return deserializer_impl<Tp>{}(deserializer);
	} else if constexpr (user_defined_deserializable<Tp, D>) {
		return T(deserializer);
	} else {
		not_deserializable_error<Tp>();
	}
}

}

#endif
