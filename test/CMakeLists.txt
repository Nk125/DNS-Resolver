cmake_minimum_required(VERSION 3.8)

project(DNSTest)

include(CTest)

add_executable(DNSTest "main.cpp")

target_compile_features(DNSTest PUBLIC cxx_std_17)

target_link_libraries(DNSTest PRIVATE DNS::Resolver)

add_test(NAME DNSTest COMMAND DNSTest "github.com" "http")