/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_SIZE_WRITE_ITERATOR_H
#define SERIALIZEPP_SIZE_WRITE_ITERATOR_H

#include <iterator>
#include <cstddef>

namespace spp::detail {

template<typename T>
class size_write_iterator {

public:
	using iterator_category = std::output_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;

	constexpr size_write_iterator() noexcept : _size(nullptr) {}
	constexpr size_write_iterator(std::size_t* sz) noexcept : _size(sz) {}

	constexpr value_type& operator*() noexcept { return _dummy; }

	constexpr size_write_iterator<T>& operator++() noexcept {
		(*_size)++;
		return *this;
	}

	constexpr size_write_iterator<T> operator++(int) noexcept {
		auto copy = *this;
		(*_size)++;
		return copy;
	}

	constexpr std::size_t size() const noexcept { return *_size; }

private:
	T _dummy{};
	std::size_t* _size;

};

}

#endif
