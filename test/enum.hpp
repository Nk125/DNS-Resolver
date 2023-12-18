#pragma once
#include <DNS Resolver.hpp>
#include <string>

#define enumValToStr(x) case static_cast<int>(x): return #x

std::string enumFamily(nk125::DNS::Hints::Family enV) {
	using namespace nk125::DNS::Hints;

	switch (static_cast<int>(enV)) {
		enumValToStr(Family::Any);
		enumValToStr(Family::IPv4);
		enumValToStr(Family::IPv6);
	}

	return "Unknown field" + std::to_string(static_cast<int>(enV));
}

std::string enumProto(nk125::DNS::Hints::Protocol enV) {
	using namespace nk125::DNS::Hints;

	switch (static_cast<int>(enV)) {
		enumValToStr(Protocol::Any);
		enumValToStr(Protocol::TCP);
		enumValToStr(Protocol::UDP);
	}

	return "Unknown field" + std::to_string(static_cast<int>(enV));
}

std::string enumSocket(nk125::DNS::Hints::Socket enV) {
	using namespace nk125::DNS::Hints;

	switch (static_cast<int>(enV)) {
		enumValToStr(Socket::Any);
		enumValToStr(Socket::Datagram);
		enumValToStr(Socket::Stream);
		enumValToStr(Socket::Raw);
	}

	return "Unknown field " + std::to_string(static_cast<int>(enV));
}