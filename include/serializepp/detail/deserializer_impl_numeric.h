/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_DESERIALIZER_IMPL_NUMERIC_H
#define SERIALIZEPP_DESERIALIZER_IMPL_NUMERIC_H

#include <bit>
#include <cstring>

namespace spp::detail {

template<>
struct deserializer_impl<std::uint8_t> {
	template<typename D>
	constexpr std::uint8_t operator()(D& input) const noexcept {
		std::uint8_t b0;
		input(b0);

		return static_cast<std::uint8_t>(b0);
	}
};

template<>
struct deserializer_impl<std::uint16_t> {
	template<typename D>
	constexpr std::uint16_t operator()(D& input) const noexcept {
		std::uint8_t b0, b1;
		input(b0, b1);

		std::uint16_t value = 0;

		if constexpr (D::byte_order == std::endian::little) {
			value |= static_cast<std::uint16_t>(b0) << 0u;
			value |= static_cast<std::uint16_t>(b1) << 8u;
		} else {
			value |= static_cast<std::uint16_t>(b1) << 8u;
			value |= static_cast<std::uint16_t>(b0) << 0u;
		}

		return value;
	}
};

template<>
struct deserializer_impl<std::uint32_t> {
	template<typename D>
	constexpr std::uint32_t operator()(D& input) const noexcept {
		std::uint8_t b0, b1, b2, b3;
		input(b0, b1, b2, b3);

		std::uint32_t value = 0;

		if constexpr (D::byte_order == std::endian::little) {
			value |= static_cast<std::uint32_t>(b0) << 0u;
			value |= static_cast<std::uint32_t>(b1) << 8u;
			value |= static_cast<std::uint32_t>(b2) << 16u;
			value |= static_cast<std::uint32_t>(b3) << 24u;
		} else {
			value |= static_cast<std::uint32_t>(b3) << 24u;
			value |= static_cast<std::uint32_t>(b2) << 16u;
			value |= static_cast<std::uint32_t>(b1) << 8u;
			value |= static_cast<std::uint32_t>(b0) << 0u;
		}

		return value;
	}
};

template<>
struct deserializer_impl<std::uint64_t> {
	template<typename D>
	constexpr std::uint64_t operator()(D& input) const noexcept {
		std::uint8_t b0, b1, b2, b3, b4, b5, b6, b7;
		input(b0, b1, b2, b3, b4, b5, b6, b7);

		std::uint64_t value = 0;

		if constexpr (D::byte_order == std::endian::little) {
			value |= static_cast<std::uint64_t>(b0) << 0u;
			value |= static_cast<std::uint64_t>(b1) << 8u;
			value |= static_cast<std::uint64_t>(b2) << 16u;
			value |= static_cast<std::uint64_t>(b3) << 24u;
			value |= static_cast<std::uint64_t>(b4) << 32u;
			value |= static_cast<std::uint64_t>(b5) << 40u;
			value |= static_cast<std::uint64_t>(b6) << 48u;
			value |= static_cast<std::uint64_t>(b7) << 56u;
		} else {
			value |= static_cast<std::uint64_t>(b7) << 56u;
			value |= static_cast<std::uint64_t>(b6) << 48u;
			value |= static_cast<std::uint64_t>(b5) << 40u;
			value |= static_cast<std::uint64_t>(b4) << 32u;
			value |= static_cast<std::uint64_t>(b3) << 24u;
			value |= static_cast<std::uint64_t>(b2) << 16u;
			value |= static_cast<std::uint64_t>(b1) << 8u;
			value |= static_cast<std::uint64_t>(b0) << 0u;
		}

		return value;
	}
};

template<typename T>
struct unsignedcast_deserializer_impl {
	template<typename D>
	constexpr T operator()(D& input) const noexcept {
		return (signed) deserializer_impl<std::make_unsigned_t<T>>{}(input);
	}
};

template<typename To, typename From>
struct staticcast_deserializer_impl {
	template<typename D>
	constexpr To operator()(D& input) const noexcept {
		return static_cast<To>(deserializer_impl<From>{}(input));
	}
};

template<typename To, typename From>
	requires (sizeof(To) == sizeof(From))
struct bitcast_deserializer_impl {
	template<typename D>
	constexpr To operator()(D& input) const noexcept {
		From from = deserializer_impl<From>{}(input);

		To to;
		std::memcpy(&to, &from, sizeof(From));
		return to;
	}
};

template<>
struct deserializer_impl<std::int8_t> : unsignedcast_deserializer_impl<std::int8_t> {};

template<>
struct deserializer_impl<std::int16_t> : unsignedcast_deserializer_impl<std::int16_t> {};

template<>
struct deserializer_impl<std::int32_t> : unsignedcast_deserializer_impl<std::int32_t> {};

template<>
struct deserializer_impl<std::int64_t> : unsignedcast_deserializer_impl<std::int64_t> {};

template<>
struct deserializer_impl<std::byte> : staticcast_deserializer_impl<std::byte, std::uint8_t> {};

template<>
struct deserializer_impl<char> : staticcast_deserializer_impl<char, std::uint8_t> {};

template<>
struct deserializer_impl<wchar_t> : staticcast_deserializer_impl<char, std::uint32_t> {};

template<>
struct deserializer_impl<char8_t> : staticcast_deserializer_impl<char8_t, std::uint8_t> {};

template<>
struct deserializer_impl<char16_t> : staticcast_deserializer_impl<char16_t, std::uint16_t> {};

template<>
struct deserializer_impl<char32_t> : staticcast_deserializer_impl<char32_t, std::uint32_t> {};

template<>
struct deserializer_impl<bool> : staticcast_deserializer_impl<bool, std::uint8_t> {};

template<>
struct deserializer_impl<float> : bitcast_deserializer_impl<float, std::uint32_t> {};

template<>
struct deserializer_impl<double> : bitcast_deserializer_impl<double, std::uint64_t> {};

}

#endif
