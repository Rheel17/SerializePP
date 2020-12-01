/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_DESERIALIZER_IMPL_CONTAINER_H
#define SERIALIZEPP_DESERIALIZER_IMPL_CONTAINER_H

#include "../type_pass.h"

#include <ranges>

namespace spp::detail {

template<container T>
struct deserializer_impl<T> {
	template<typename D>
	constexpr T operator()(D& input) const noexcept {
		T values{};
		auto size = input(type < std::uint64_t > {});

		// TODO: check whether size fits std::size_t

		if constexpr (has_reserve < T >) {
			values.reserve(size);
		}

		for (std::size_t i = 0; i < size; i++) {
			if constexpr (push_back_container < T >) {
				values.push_back(input(type < T > {}));
			} else if constexpr (insert_container < T >) {
				values.insert(input(type < T > {}));
			}
		}
	}
};

template<typename T, std::size_t N>
struct deserializer_impl<std::array<T, N>> {
	template<typename D>
	constexpr std::array<T, N> operator()(D& input) const noexcept {
		if constexpr (std::is_default_constructible_v<T> && std::is_nothrow_move_assignable_v<T>) {
			std::array<T, N> values{};

			for (std::size_t i = 0; i < N; i++) {
				input(values[i]);
			}

			return values;
		} else {
			return deserialize_immediate(input, std::make_index_sequence<N>());
		}
	}

	template<typename D, std::size_t... I>
	constexpr std::array<T, N> deserialize_immediate(D& input, std::index_sequence<I...>) const noexcept {
		return { (I, input(type < T > {}))... };
	}
};

}

#endif
