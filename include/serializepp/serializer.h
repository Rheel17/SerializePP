/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_SERIALIZER_H
#define SERIALIZEPP_SERIALIZER_H

#include "detail/serializer_impl.h"

#include <memory>

namespace spp {

class serializer_base {
	template<detail::byte_output_iterator, std::endian, typename>
	friend class serializer;
	serializer_base() = default;
};

template<detail::byte_output_iterator OutputIt, std::endian byte_order_value, typename Capture = void>
class serializer : public serializer_base {

public:
	constexpr static std::endian byte_order = byte_order_value;

	constexpr explicit serializer(OutputIt it) requires (std::is_void_v<Capture>) :
			_writer(it) {}

	constexpr serializer(OutputIt it, std::unique_ptr<Capture> capture) requires (!std::is_void_v<Capture>) :
			_writer(it),
			_capture(std::move(capture)) {}

	template<typename... Ts>
	constexpr void operator()(const Ts& ... values) {
		(_serialize(values), ...);
	}

	constexpr void operator()() = delete;

private:
	template<typename T>
	constexpr void _serialize(const T& value) {
		if constexpr (std::same_as<std::remove_cvref_t<T>, std::uint8_t>) {
			_add_byte(value);
		} else {
			detail::call_serializer(*this, value);
		}
	}

	constexpr void _add_byte(std::uint8_t byte) {
		if constexpr (detail::has_iter_value_t<OutputIt>) {
			*_writer++ = static_cast<std::iter_value_t<OutputIt>>(byte);
		} else {
			*_writer++ = byte;
		}
	}

private:
	OutputIt _writer;
	std::conditional_t<std::is_void_v<Capture>, int, std::unique_ptr<Capture>> _capture{};
};

}

#endif
