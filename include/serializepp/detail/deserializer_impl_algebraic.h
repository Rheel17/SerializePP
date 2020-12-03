/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_DESERIALIZER_IMPL_ALGEBRAIC_H
#define SERIALIZEPP_DESERIALIZER_IMPL_ALGEBRAIC_H

#include "../type_pass.h"

#include <concepts>

namespace spp::detail {

template<optional_like T>
struct deserializer_impl<T> {
	template<typename D>
	constexpr T operator()(D& input) const noexcept {
		bool has_value;
		input(has_value);

		if (has_value) {
			return input(type<typename T::value_type>{});
		} else {
			return T{};
		}
	}
};

template<tuple_like T>
struct deserializer_impl<T> {
	template<typename D>
	constexpr T operator()(D& input) const noexcept {
		return deserialize(input, std::make_index_sequence<std::tuple_size_v<T>>());
	}

	template<typename D, std::size_t... I>
	constexpr T deserialize(D& input, std::index_sequence<I...>) const noexcept {
		return { input(type<std::tuple_element_t<I, T>>{})... };
	}
};

}

#endif
