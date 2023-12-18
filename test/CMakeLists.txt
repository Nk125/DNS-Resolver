cmake_minimum_required(VERSION 3.8)

project(DNSTest)

add_executable(DNSTest "main.cpp")

target_compile_features(DNSTest PUBLIC cxx_std_11)

target_link_libraries(DNSTest PRIVATE DNS::Resolver)
