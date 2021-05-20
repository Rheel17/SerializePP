/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_SERIALIZER_IMPL_ENUM_H
#define SERIALIZEPP_SERIALIZER_IMPL_ENUM_H

#include <type_traits>

namespace spp::detail {

template<typename E>
	requires std::is_enum_v<E>
struct serializer_impl<E> {
	template<typename S>
	constexpr void operator()(S& output, E value) const {
		output(static_cast<std::underlying_type_t<E>>(value));
	}
};

}

#endif
