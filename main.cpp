#include <iostream>
#include <vector>
#include <array>

#include <fmt/format.h>

#include "include/serializepp/spp.h"

struct test {
	int foo = 0;
	float bar = 0.0f;

	test(int f, float b) :
			foo(f),
			bar(b) {}

	test() = default;

	SPP_IMPLEMENT_DEFAULT(test, foo, bar);
};

int main() {
	auto serialize = spp::serialize_to("test.bin");
	serialize("Hello", test{ 0x12345678, 0.1f }, std::vector<int>{ 3, 42, 195, 1046 }, std::nullopt, std::make_tuple('a', true), std::make_optional(42));

	auto deserialize = spp::deserialize_from("test.bin");
	// std::array<char, 6>
	// deserialize.deserialize<>(spp::ty);
}
