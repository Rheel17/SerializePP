/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_SPP_H
#define SERIALIZEPP_SPP_H

#include "serializer.h"
#include "deserializer.h"

#include <fstream>
#include <filesystem>

namespace spp {

/////////////////
// serializers //
/////////////////

template<std::endian byte_order = std::endian::native>
constexpr auto serialize_to(detail::byte_output_iterator auto iterator) noexcept {
	return serializer<decltype(iterator), byte_order>(iterator);
}

template<std::endian byte_order = std::endian::native>
constexpr auto serialize_to(detail::byte_output_range auto& range) noexcept {
	return serialize_to<byte_order>(std::ranges::begin(range));
}

template<std::endian byte_order = std::endian::native, typename Char, typename CharTraits>
constexpr auto serialize_to(std::basic_ostream<Char, CharTraits>& stream) noexcept {
	auto iterator = std::ostream_iterator<std::uint8_t>(stream);
	return serialize_to<byte_order>(iterator);
}

template<std::endian byte_order = std::endian::native>
auto serialize_to(const std::filesystem::path& file) noexcept {
	auto stream = std::make_unique<std::ofstream>(file);
	auto iterator = std::ostream_iterator<std::uint8_t>(*stream);
	return serializer<decltype(iterator), byte_order, std::ofstream>(iterator, std::move(stream));
}

///////////////////
// deserializers //
///////////////////

template<std::endian byte_order = std::endian::native>
auto deserialize_from(detail::byte_input_iterator auto iterator) {
	return deserializer<decltype(iterator), byte_order>(iterator);
}

template<std::endian byte_order = std::endian::native>
constexpr auto deserialize_from(detail::byte_input_range auto& range) noexcept {
	return deserialize_from<byte_order>(std::ranges::begin(range));
}

template<std::endian byte_order = std::endian::native, typename Char, typename CharTraits>
constexpr auto deserialize_from(std::basic_istream<Char, CharTraits>& stream) noexcept {
	auto iterator = std::istream_iterator<std::uint8_t>(stream);
	return deserialize_from<byte_order>(iterator);
}

template<std::endian byte_order = std::endian::native>
auto deserialize_from(const std::filesystem::path& file) noexcept {
	auto stream = std::make_unique<std::ifstream>(file);
	auto iterator = std::istream_iterator<std::uint8_t>(*stream);
	return deserializer<decltype(iterator), byte_order, std::ifstream>(iterator, std::move(stream));
}

template<std::endian byte_order = std::endian::native>
auto deserialize_from(const char* filename) noexcept {
	return deserialize_from<byte_order>(std::filesystem::path(filename));
}

}

#define SPP_IMPLEMENT_DEFAULT(Type, ...)                                \
    template<typename Deserializer>                                     \
    constexpr explicit Type(Deserializer& deserialize) noexcept {       \
        deserialize(__VA_ARGS__);                                       \
    }                                                                   \
                                                                        \
    template<typename Serializer>                                       \
    constexpr void serialize(Serializer& serialize) const noexcept {    \
        serialize(__VA_ARGS__);                                         \
    }

#endif
