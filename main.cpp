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

template<std::endian E>
struct endian_string {};

template<>
struct endian_string<std::endian::little> {
	static inline std::string name = "little";
};

template<>
struct endian_string<std::endian::big> {
	static inline std::string name = "big";
};

template<std::endian E>
void test_twoway() {
	std::string filename = "test." + endian_string<E>::name + ".bin";

	{
		auto serialize = spp::serialize_to<E>(std::filesystem::path(filename));
		serialize("Hello", test{ 0x12345678, 0.1f }, std::vector<int>{ 3, 42, 195, 1046 }, std::nullopt, std::make_tuple('a', true),
				std::make_optional(-788532100983063651ll));
	}

	auto deserialize = spp::deserialize_from<E>(std::filesystem::path(filename));
	auto[hello, test_inst, vec, nullopt, tupl, opt] = deserialize(
			spp::types<std::array<char, 6>, test, std::vector<int>, std::optional<float>, std::tuple<char, bool>, std::optional<long long>>{});

	assert((hello == std::array<char, 6>{ 'H', 'e', 'l', 'l', 'o', 0 }));
	assert(test_inst.foo == 0x12345678);
	assert(test_inst.bar == 0.1f);
	assert((vec == std::vector<int>{ 3, 42, 195, 1046 }));
	assert(nullopt == std::nullopt);
	assert(std::get<0>(tupl) == 'a');
	assert(std::get<1>(tupl) == true);
	assert(opt == std::make_optional(-788532100983063651ll));

	std::cout << "deserialization success for " << filename << std::endl;
}

int main() {
	test_twoway<std::endian::little>();
	test_twoway<std::endian::big>();
}
