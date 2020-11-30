/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef SERIALIZEPP_DESERIALIZER_H
#define SERIALIZEPP_DESERIALIZER_H

#include "deserializer_impl.h"

#include <memory>

namespace spp::detail {

template<detail::byte_input_iterator InputIt>
struct iterator_reader {
	InputIt it;

	constexpr std::uint8_t read_byte() noexcept {
		if constexpr (std::output_iterator<InputIt, std::uint8_t>) {
			return *it++;
		} else {
			return static_cast<std::uint8_t>(*it++);
		}
	}
};

template<detail::byte_input_iterator InputIt, std::endian byte_order_value>
class deserializer {
	using reader_type = iterator_reader<InputIt>;

public:
	constexpr static std::endian byte_order = byte_order_value;

	constexpr explicit deserializer(InputIt it) noexcept:
			_reader{ it } {}

	template<typename... Ts>
	constexpr void operator()(Ts& ... values) noexcept {
		(_deserialize(values), ...);
	}

	template<typename... Ts>
	constexpr std::tuple<Ts...> operator()() noexcept {
		
	}

private:
	template<typename T>
	constexpr void _serialize(const T& value) noexcept {
		if constexpr (std::same_as<std::remove_cvref_t<T>, std::uint8_t>) {
			_reader.add_byte(value);
		} else {
			call_serializer(*this, value);
		}
	}

private:
	reader_type _reader;
};

}

#endif
