/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SERIALIZEPP_CONCEPTS_H
#define SERIALIZEPP_CONCEPTS_H

#include <bit>
#include <iterator>
#include <ranges>

namespace spp::detail {

////////////////////////////////////
// (de)serializer_impl definition //
////////////////////////////////////

template<typename>
struct serializer_impl;

template<typename>
struct deserializer_impl;

////////////////////////
// interface concepts //
////////////////////////

template<typename T, typename S>
concept builtin_serializable = requires(S& serializer, const T& value) {
	{ serializer_impl<T>{}(serializer, value) };
};

template<typename T, typename D>
concept builtin_deserializable = requires(D& deserializer) {
	{ deserializer_impl<T>{}(deserializer) } -> std::convertible_to<T>;
};

template<typename T, typename S>
concept user_defined_serializable = requires(S& serializer, const T& value) {
	{ value.serialize(serializer) };
};

template<typename T, typename D>
concept user_defined_deserializable_constructor = requires(D& deserializer) {
	{ T(deserializer) };
};

template<typename T, typename D>
concept user_defined_deserializable_method = requires(D& deserializer) {
	{ T::deserialize(deserializer) } -> std::convertible_to<T>;
};

///////////////////
// byte concepts //
///////////////////

template<typename T>
concept has_iter_value_t = requires {
	typename std::iter_value_t<T>;
};

template<typename T>
concept byte_like = std::convertible_to<T, std::byte> || (sizeof(T) == 1 && std::integral<T>);

template<typename I>
concept byte_output_iterator = std::output_iterator<I, std::byte> || std::output_iterator<I, std::uint8_t> || std::output_iterator<I, std::int8_t>;

template<typename R>
concept byte_output_range = std::ranges::range<R> && byte_output_iterator<std::ranges::iterator_t<R>>;

template<typename I>
concept byte_input_iterator = std::input_iterator<I> && byte_like<std::iter_value_t<I>>;

template<typename R>
concept byte_input_range = std::ranges::range<R> && byte_input_iterator<std::ranges::iterator_t<R>>;

////////////////////////////////////////////////////////////////////////////////
//                      USED FOR BUILT-IN (DE)SERIALIZERS                     //
////////////////////////////////////////////////////////////////////////////////

////////////////
// containers //
////////////////

template<typename T>
concept push_back_container = std::is_default_constructible_v<T> && requires {
	typename T::value_type;
} && requires(T& container, typename T::value_type&& value) {
	{ container.push_back(std::move(value)) };
};

template<typename T>
concept insert_container = std::is_default_constructible_v<T> && requires {
	typename T::value_type;
} && requires(T& container, typename T::value_type&& value) {
	{ container.insert(std::move(value)) };
};

template<typename T>
concept container = push_back_container<T> || insert_container<T>;

template<typename T>
concept has_reserve = requires(T& container, std::size_t size) {
	{ container.reserve(size) };
};

///////////////
// optionals //
///////////////

template<typename T>
concept optional_has_value = requires(const T t) {
	{ t.has_value() } -> std::convertible_to<bool>;
};

template<typename T>
concept optional_value = requires(const T t) {
	typename T::value_type;
	{ t.value() } -> std::convertible_to<typename T::value_type>;
};

template<typename T>
concept optional_like = (optional_has_value<T> || std::convertible_to<T, bool>) && optional_value<T> &&
		std::is_default_constructible_v<T> && std::constructible_from<T, typename T::value_type>;

////////////
// tuples //
////////////

template<typename T, std::size_t I>
concept tuple_single_element_and_get = requires(const T t) {
	typename std::tuple_element<I, T>::type;
	{ std::get<I>(t) } -> std::convertible_to<typename std::tuple_element<I, T>::type>;
};

template<typename T, std::size_t... I>
constexpr bool tuple_has_element_and_get(std::index_sequence<I...>) {
	return (tuple_single_element_and_get<T, I> && ... && true);
}

template<typename T, std::size_t I>
concept is_std_array = std::greater{}(I, std::size_t{0}) &&
	tuple_single_element_and_get<T, 0> && 
	std::same_as<T, std::array<std::tuple_element_t<0, T>, I>>;

template<typename T>
concept tuple_like = requires {
	{ std::tuple_size<T>{} };
	{ std::tuple_size_v<T> } -> std::convertible_to<std::size_t>;
} && !is_std_array<T, std::tuple_size_v<T>>
  && tuple_has_element_and_get<T>(std::make_index_sequence<std::tuple_size_v<T>>());

}

#endif

