#include <iostream>
#include <vector>
#include <array>
#include <cassert>
#include <iomanip>

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
	constexpr const auto* filename = "test.bin";

	{
		auto serialize = spp::serialize_to(filename);
		serialize("Hello", test{ 0x12345678, 0.1f }, std::vector<int>{ 3, 42, 195, 1046 }, std::nullopt, std::make_tuple('a', true), std::make_optional(42));
	}

	auto deserialize = spp::deserialize_from(filename);
	auto[hello, test_inst, vec, nullopt, tupl, opt] = deserialize(
			spp::types<std::array<char, 6>, test, std::vector<int>, std::optional<float>, std::tuple<char, bool>, std::optional<int>>{});

	assert((hello == std::array<char, 6>{'H', 'e', 'l', 'l', 'o', 0}));
	assert(test_inst.foo == 0x12345678);
	assert(test_inst.bar == 0.1f);
	assert((vec == std::vector<int>{ 3, 42, 195, 1046 }));
	assert(nullopt == std::nullopt);
	assert(std::get<0>(tupl) == 'a');
	assert(std::get<1>(tupl) == true);
	assert(opt == std::make_optional(42));

	std::cout << "deserialization success" << std::endl;
}
