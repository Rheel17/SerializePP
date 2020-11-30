/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_SERIALIZER_IMPL_H
#define SERIALIZEPP_SERIALIZER_IMPL_H

#include "concepts.h"
#include "value_identity.h"

namespace spp::detail {

template<typename S, typename T>
constexpr void call_serializer(S& serializer, const T& value) noexcept;

}

#include "serializer_impl_container.h"
#include "serializer_impl_numeric.h"
#include "serializer_impl_algebraic.h"

namespace spp::detail {

template<typename T>
void not_serializable_error() {
	static_assert(indirect_false_v<T>, R"(
Type is not serializable!

For user-defined types to be serializable, add a public method

	template<typename Serializer>
	constexpr void serialize(Serializer& s) const noexcept {
		// ...
	}

to the type.

)");
}

template<typename S, typename T>
constexpr void call_serializer(S& serializer, const T& value) noexcept {
	using Tp = std::remove_cvref_t<T>;

	if constexpr (builtin_serializable<Tp, S>) {
		serializer_impl<Tp>{}(serializer, value);
	} else if constexpr (user_defined_serializable<Tp, S>) {
		value.serialize(serializer);
	} else {
		not_serializable_error<Tp>();
	}
}

}

#endif
