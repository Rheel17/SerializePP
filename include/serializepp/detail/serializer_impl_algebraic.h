/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_SERIALIZER_IMPL_ALGEBRAIC_H
#define SERIALIZEPP_SERIALIZER_IMPL_ALGEBRAIC_H

#include <concepts>

namespace spp::detail {

template<optional_like T>
struct serializer_impl<T> {
	template<typename S>
	constexpr void operator()(S& output, const T& opt) const {
		bool has_value{};
		if constexpr (optional_has_value<T>) {
			has_value = opt.has_value();
		} else if constexpr (std::convertible_to<T, bool>) {
			has_value = static_cast<bool>(opt);
		}

		output(has_value);
		if (has_value) {
			output(opt.value());
		}
	}
};

template<>
struct serializer_impl<std::nullopt_t> {
	template<typename S>
	constexpr void operator()(S& output, std::nullopt_t) const {
		output(false);
	}
};

template<tuple_like T>
struct serializer_impl<T> {
	template<typename S>
	constexpr void operator()(S& output, const T& tuple) const {
		serialize_elements(output, tuple, std::make_index_sequence<std::tuple_size_v<T>>());
	}

	template<typename S, std::size_t... I>
	constexpr void serialize_elements(S& output, const T& tuple, std::index_sequence<I...>) const {
		(output(std::get<I>(tuple)), ...);
	}
};

}

#endif
