/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_SPP_H
#define SERIALIZEPP_SPP_H

#include "serializer.h"
#include "deserializer.h"
#include "detail/size_write_iterator.h"
#include "detail/stream_iterator.h"

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
	auto iterator = std::ostream_iterator<std::uint8_t> (stream);
	return serialize_to<byte_order>(iterator);
}

template<std::endian byte_order = std::endian::native>
auto serialize_to(const std::filesystem::path& file) noexcept {
	auto stream = std::make_unique<std::ofstream>(file, std::ios::binary);
	auto iterator = std::ostream_iterator<std::uint8_t>(*stream);
	return serializer<decltype(iterator), byte_order, std::ofstream>(iterator, std::move(stream));
}

////////////////////////
// serialization size //
////////////////////////

template<typename... T>
constexpr std::size_t serialization_size(const T&... args) noexcept {
	if constexpr (sizeof...(T) == 0) {
		return 0;
	} else {
		std::size_t size = 0;
		detail::size_write_iterator<std::uint8_t> dummy_iterator(&size);

		auto serializer = serialize_to(dummy_iterator);
		serializer(args...);
		
		return dummy_iterator.size();
	}
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
	auto iterator = detail::istream_iterator<Char, CharTraits>(stream);
	return deserialize_from<byte_order>(iterator);
}

template<std::endian byte_order = std::endian::native>
auto deserialize_from(const std::filesystem::path& file) noexcept {
	auto stream = std::make_unique<std::ifstream>(file, std::ios::binary);
	auto iterator = detail::istream_iterator<std::ifstream::char_type, std::ifstream::traits_type>(*stream);
	return deserializer<decltype(iterator), byte_order, std::ifstream>(iterator, std::move(stream));
}

template<std::endian byte_order = std::endian::native>
auto deserialize_from(const char* filename) noexcept {
	return deserialize_from<byte_order>(std::filesystem::path(filename));
}

}

#define SPP_IMPLEMENT_DEFAULT_DESERIALIZER(Type, ...)                   \
    template<typename Deserializer>                                     \
    constexpr explicit Type(Deserializer& deserialize) noexcept {       \
        deserialize(__VA_ARGS__);                                       \
    }                                                                   \


#define SPP_IMPLEMENT_DEFAULT_SERIALIZER(Type, ...)                     \
    template<typename Serializer>                                       \
    constexpr void serialize(Serializer& serialize) const noexcept {    \
        serialize(__VA_ARGS__);                                         \
    }

#define SPP_EXPAND(x) x

#define SPP_IMPLEMENT_DEFAULT(Type, ...)                                \
	SPP_IMPLEMENT_DEFAULT_DESERIALIZER(Type, __VA_ARGS__)           \
	SPP_IMPLEMENT_DEFAULT_SERIALIZER(Type, __VA_ARGS__)

#endif
