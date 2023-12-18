cmake_minimum_required(VERSION 3.8)

project(DNSResolver)

option(BUILD_TEST "Compiles a test program" OFF)

# Sets up the library as only interface
add_library(DNSResolver INTERFACE)

target_compile_features(DNSResolver INTERFACE cxx_std_11)

target_include_directories(DNSResolver INTERFACE "include")

# Links Winsock in Windows builds
if(WIN32)
	target_link_libraries(DNSResolver INTERFACE wsock32 ws2_32)

	# Adds a flag to acknowledge the library is being configured with CMake, instead,
	#	links itself with winsock2
	#
	# The other value determines a target Windows version, set up by default in Windows 7
	target_compile_definitions(DNSResolver INTERFACE IS_CMAKE_BUILD _WIN32_WINNT=0x0601)
	
	message(STATUS "Windows build")
else()
	message(STATUS "Unix build")
endif()

add_library(DNS::Resolver ALIAS DNSResolver)

if (BUILD_TEST)
	message(STATUS "Configuring test")
	add_subdirectory("test")
endif()

# Else if you don't have SDK, this could be an alternative:
#find_library(ws2_32_LIBRARY_PATH ws2_32)
#target_link_libraries(CMakeTarget INTERFACE ${ws2_32_LIBRARY_PATH})