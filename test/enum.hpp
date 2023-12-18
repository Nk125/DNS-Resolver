#pragma once
#include <DNS Resolver.hpp>
#include <string>

#define enumValToStr(x) case x: return #x

template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
std::string enumFamily(T enV) {
	using namespace nk125::DNS::Hints;

	switch (static_cast<int>(enV)) {
		enumValToStr(Family::Any);
		enumValToStr(Family::IPv4);
		enumValToStr(Family::IPv6);
	}

	return "Unknown field" + std::to_string(static_cast<int>(enV));
}

template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
std::string enumProto(T enV) {
	using namespace nk125::DNS::Hints;

	switch (static_cast<int>(enV)) {
		enumValToStr(Protocol::Any);
		enumValToStr(Protocol::TCP);
		enumValToStr(Protocol::UDP);
	}

	return "Unknown field" + std::to_string(static_cast<int>(enV));
}

template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
std::string enumSocket(T enV) {
	using namespace nk125::DNS::Hints;

	switch (static_cast<int>(enV)) {
		enumValToStr(Socket::Any);
		enumValToStr(Socket::Datagram);
		enumValToStr(Socket::Stream);
		enumValToStr(Socket::Raw);
	}

	return "Unknown field " + std::to_string(static_cast<int>(enV));
}