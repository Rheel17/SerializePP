/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef SERIALIZEPP_TYPE_PASS_H
#define SERIALIZEPP_TYPE_PASS_H

namespace spp {

template<typename T>
struct type {};

template<typename... Ts>
struct types {};

}

#endif
