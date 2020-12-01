/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef SERIALIZEPP_DESERIALIZER_H
#define SERIALIZEPP_DESERIALIZER_H

#include "type_pass.h"
#include "detail/deserializer_impl.h"

#include <memory>

namespace spp {

template<detail::byte_input_iterator InputIt, std::endian byte_order_value, typename Capture = void>
class deserializer {

public:
	constexpr static std::endian byte_order = byte_order_value;

	constexpr explicit deserializer(InputIt it) noexcept:
			_reader(it) {}

	constexpr deserializer(InputIt it, std::unique_ptr<Capture> capture) noexcept:
			_reader(it),
			_capture(std::move(capture)) {}

	template<typename T>
	constexpr T operator()(type<T>) {
		return _deserialize<T>();
	}

	template<typename... Ts>
	constexpr std::tuple<Ts...> operator()(types<Ts...>) {
		return { _deserialize<Ts>()... };
	}

	template<typename T1, typename... Ts>
	constexpr void operator()(T1& value, Ts& ... values) noexcept {
		std::tie(value, values...) = operator()(types<T1, Ts...>{});
	}

private:
	template<typename T>
	constexpr T _deserialize() noexcept {
		if constexpr (std::same_as<std::remove_cvref_t<T>, std::uint8_t>) {
			return _read_byte();
		} else {
			return detail::call_deserializer<T>(*this);
		}
	}

	constexpr std::uint8_t _read_byte() noexcept {
		return static_cast<std::uint8_t>(*_reader++);
	}

	InputIt _reader;
	std::unique_ptr<Capture> _capture;
};

}

#endif
