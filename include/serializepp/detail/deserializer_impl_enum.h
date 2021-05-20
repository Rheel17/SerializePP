/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_DESERIALIZER_IMPL_ENUM_H
#define SERIALIZEPP_DESERIALIZER_IMPL_ENUM_H

#include <type_traits>

namespace spp::detail {

template<typename E>
	requires std::is_enum_v<E>
struct deserializer_impl<E> {
	template<typename D>
	constexpr E operator()(D& input) const {
		std::underlying_type_t<E> value;
		input(value);

		return static_cast<E>(value);
	}
};

}

#endif
