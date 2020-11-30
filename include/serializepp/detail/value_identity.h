/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_VALUE_IDENTITY_H
#define SERIALIZEPP_VALUE_IDENTITY_H

namespace spp::detail {

template<typename>
struct indirect_false {
	static constexpr bool value = false;
};

template<typename T>
constexpr bool indirect_false_v = indirect_false<T>::value;

}

#endif
