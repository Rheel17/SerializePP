/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_SERIALIZER_IMPL_CONTAINER_H
#define SERIALIZEPP_SERIALIZER_IMPL_CONTAINER_H

#include <ranges>

namespace spp::detail {

template<std::ranges::input_range R>
struct serializer_impl<R> {
	template<typename S>
	constexpr void operator()(S& output, const R& range) const {
		output(static_cast<std::uint64_t>(std::ranges::size(range)));

		for (const auto& element : range) {
			output(element);
		}
	}
};

template<typename T, std::size_t N>
struct serializer_impl<T[N]> {
	template<typename S>
	constexpr void operator()(S& output, const T* values) const {
		for (std::size_t i = 0; i < N; i++) {
			output(values[i]);
		}
	}
};

template<typename T, std::size_t N>
struct serializer_impl<std::array<T, N>> {
	template<typename S>
	constexpr void operator()(S& output, const std::array<T, N>& array) const {
		for (std::size_t i = 0; i < N; i++) {
			output(array[i]);
		}
	}
};

}

#endif
