/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_SERIALIZER_IMPL_NUMERIC_H
#define SERIALIZEPP_SERIALIZER_IMPL_NUMERIC_H

#include <bit>
#include <cstring>

namespace spp::detail {

template<>
struct serializer_impl<std::uint8_t> {
	template<typename S>
	constexpr void operator()(S& output, std::uint8_t value) const noexcept {
		output(static_cast<std::uint8_t>(value));
	}
};

template<>
struct serializer_impl<std::uint16_t> {
	template<typename S>
	constexpr void operator()(S& output, std::uint16_t value) const noexcept {
		if constexpr (S::byte_order == std::endian::little) {
			output(static_cast<std::uint8_t>((value >> 0u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 8u) & 0xFFu));
		} else {
			output(static_cast<std::uint8_t>((value >> 8u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 0u) & 0xFFu));
		}
	}
};

template<>
struct serializer_impl<std::uint32_t> {
	template<typename S>
	constexpr void operator()(S& output, std::uint32_t value) const noexcept {
		if constexpr (S::byte_order == std::endian::little) {
			output(static_cast<std::uint8_t>((value >> 0u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 8u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 16u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 24u) & 0xFFu));
		} else {
			output(static_cast<std::uint8_t>((value >> 24u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 16u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 8u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 0u) & 0xFFu));
		}
	}
};

template<>
struct serializer_impl<std::uint64_t> {
	template<typename S>
	constexpr void operator()(S& output, std::uint64_t value) const noexcept {
		if constexpr (S::byte_order == std::endian::little) {
			output(static_cast<std::uint8_t>((value >> 0u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 8u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 16u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 24u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 32u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 40u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 48u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 56u) & 0xFFu));
		} else {
			output(static_cast<std::uint8_t>((value >> 56u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 48u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 40u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 32u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 24u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 16u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 8u) & 0xFFu));
			output(static_cast<std::uint8_t>((value >> 0u) & 0xFFu));
		}
	}
};

template<typename T>
struct unsignedcast_serializer_impl {
	template<typename S>
	constexpr void operator()(S& output, T from) const noexcept {
		serializer_impl<std::make_unsigned_t<T>>{}(output, (std::make_unsigned_t<T>) from);
	}
};

template<typename From, typename To>
struct staticcast_serializer_impl {
	template<typename S>
	constexpr void operator()(S& output, From from) const noexcept {
		serializer_impl<To>{}(output, static_cast<To>(from));
	}
};

template<typename From, typename To>
struct bitcast_serializer_impl {
	template<typename S>
	constexpr void operator()(S& output, From from) const noexcept {
		To to;
		std::memcpy(&to, &from, sizeof(From));
		serializer_impl<To>{}(output, to);
	}
};

template<>
struct serializer_impl<std::int8_t> : unsignedcast_serializer_impl<std::int8_t> {};

template<>
struct serializer_impl<std::int16_t> : unsignedcast_serializer_impl<std::int16_t> {};

template<>
struct serializer_impl<std::int32_t> : unsignedcast_serializer_impl<std::int32_t> {};

template<>
struct serializer_impl<std::int64_t> : unsignedcast_serializer_impl<std::int64_t> {};

template<>
struct serializer_impl<std::byte> : staticcast_serializer_impl<std::byte, std::uint8_t> {};

template<>
struct serializer_impl<char> : staticcast_serializer_impl<char, std::uint8_t> {};

template<>
struct serializer_impl<wchar_t> : staticcast_serializer_impl<char, std::uint32_t> {};

template<>
struct serializer_impl<char8_t> : staticcast_serializer_impl<char8_t, std::uint8_t> {};

template<>
struct serializer_impl<char16_t> : staticcast_serializer_impl<char16_t, std::uint16_t> {};

template<>
struct serializer_impl<char32_t> : staticcast_serializer_impl<char32_t, std::uint32_t> {};

template<>
struct serializer_impl<bool> : staticcast_serializer_impl<bool, std::uint8_t> {};

template<>
struct serializer_impl<float> : bitcast_serializer_impl<float, std::uint32_t> {};

template<>
struct serializer_impl<double> : bitcast_serializer_impl<double, std::uint64_t> {};

}

#endif
