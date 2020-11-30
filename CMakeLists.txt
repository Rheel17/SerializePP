cmake_minimum_required(VERSION 3.17)
project(SerializePP)

set(CMAKE_CXX_STANDARD 20)

add_executable(SerializePP
		main.cpp
		include/serializepp/spp.h
		include/serializepp/detail/serializer.h
		include/serializepp/detail/concepts.h
		include/serializepp/detail/serializer_impl_numeric.h
		include/serializepp/detail/serializer_impl_container.h
		include/serializepp/detail/serializer_impl.h
		include/serializepp/detail/value_identity.h
		include/serializepp/detail/serializer_impl_algebraic.h
		include/serializepp/detail/deserializer.h
		include/serializepp/detail/deserializer_impl.h
		)

target_link_libraries(SerializePP fmt)
