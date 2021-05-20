/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_STREAM_ITERATOR_H
#define SERIALIZEPP_STREAM_ITERATOR_H

#include <iterator>
#include <istream>

namespace spp::detail {

template<typename Char, typename CharTraits>
class istream_iterator {

public:
	using iterator_category = std::input_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = std::uint8_t;

	constexpr istream_iterator() = default;
	constexpr istream_iterator(std::basic_istream<Char, CharTraits>& stream) : _stream(&stream) {
		_read();
	}

	constexpr value_type& operator*() const { return _byte; }

	istream_iterator<Char, CharTraits>& operator++() {
		_read();
		return *this;
	}

	istream_iterator<Char, CharTraits> operator++(int) {
		auto copy = *this;
		_read();
		return copy;
	}

private:
	void _read() {
		if (_stream) {
			_stream->read(reinterpret_cast<Char*>(&_byte), 1);

			if (!_stream->good()) {
				_stream = nullptr;
			}
		}
	}

	mutable std::uint8_t _byte{};
	std::basic_istream<Char, CharTraits>* _stream{};

};

}

#endif

