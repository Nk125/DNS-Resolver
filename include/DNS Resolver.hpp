#pragma once
// NK125 DNS Resolver

#ifdef _WIN32
	// Windows Implementation
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <ws2tcpip.h>
	
	#ifndef IS_CMAKE_BUILD
	#pragma comment(lib, "ws2_32")
	#endif
#else
	// UNIX Implementation
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>
#endif
#include <cstring>
#include <string>
#include <system_error>
#include <vector>

namespace nk125 {
	namespace DNS {
		namespace Error {
			/*
			See a list of error codes in: https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo
			*/
			// Error classes setup
			enum class ErrorCode {
				gaiAgain = EAI_AGAIN,
				gaiBadFlags = EAI_BADFLAGS,
				gaiGeneralFail = EAI_FAIL,
				gaiFamilyUnsupported = EAI_FAMILY,
				gaiNotEnoughResources = EAI_MEMORY,
				gaiHostNotFound = EAI_NONAME,
				gaiServiceSockTypeMismatch = EAI_SERVICE,
				gaiSockTypeUnsupported = EAI_SOCKTYPE,
#ifdef _WIN32
				wSockFail = 1
#endif
			};

			class DNSErrorCategory : public std::error_category {
			public:
				const char* name() const noexcept override {
					return "DNS Hostname Resolution";
				}

				std::string message(int ev) const override {
					switch (static_cast<ErrorCode>(ev)) {
					case ErrorCode::gaiAgain:
						return "Temporarily Unavailable.";
					
					case ErrorCode::gaiBadFlags:
						return "Invalid flags value.";
					
					case ErrorCode::gaiGeneralFail:
						return "Unrecoverable general fail.";
					
					case ErrorCode::gaiFamilyUnsupported:
						return "The family provided in hints isn't supported.";
					
					case ErrorCode::gaiNotEnoughResources:
						return "Not enough system resources.";
					
					case ErrorCode::gaiHostNotFound:
						return "Host couldn't be resolved.";
					
					case ErrorCode::gaiServiceSockTypeMismatch:
						return "The service/port isn't compatible with socket type in hints or service doesn't exist.";
					
					case ErrorCode::gaiSockTypeUnsupported:
						return "Socket type of hints isn't supported.";
#ifdef _WIN32
					case ErrorCode::wSockFail:
						return "WinSock2 couldn't start properly.";
#endif
					default:
						return "Unknown error.";
					}
				}
			};

			const std::error_category& getDNSErrorCategory() noexcept {
				static DNSErrorCategory instance;
				return instance;
			}
		}

		/*
		* Following Linux specification, see https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html
		* For Windows specification (to add e.g. NetBIOS protocol) see https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo
		* I'm not 100% sure the implementation of some types like SOCK_RAW, SOCK_RDM, AF_NETBIOS, etc. in Linux/Unix so i'm just omitting those
		*/
		namespace Hints {
			template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
			int hintNum(T hintClass) {
				return static_cast<int>(hintClass);
			}

			template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
			T revHintNum(int hintNum) {
				return static_cast<T>(hintNum);
			}

			enum class Flags : uint8_t {
				None = 0,
				OnlyNumericAddress = 1 << 0,
				OnlyNumericPort = 1 << 1,
				// Sorry for windows users, the support of punycode for DNS resolution requires a fully rewrite of the lib
				// and even i couldn't find a punycode domain for testing
#ifndef _WIN32
				UseIDNPunycode = 1 << 2
#endif
			};

			enum class Protocol : int {
				TCP = IPPROTO_TCP,
				UDP = IPPROTO_UDP,
				Any = 0
			};

			enum class Socket : int {
				// Normally for UDP
				Datagram = SOCK_DGRAM,
				// Normally for TCP
				Stream = SOCK_STREAM,
				// Direct access to socket
				Raw = SOCK_RAW,
				/* Not commonly used socket types, even with all locale interfaces those doesn't show up anything so i'm removing it
				// Reliable Message Datagram https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo
				RDM = SOCK_RDM,
				// SeqPacket, for more info about Socket Types see the link above and https://www.man7.org/linux/man-pages/man2/socket.2.html#DESCRIPTION
				SeqPacket = SOCK_SEQPACKET,*/
				Any = 0
			};

			enum class Family : int {
				IPv4 = AF_INET,
				IPv6 = AF_INET6,
				Unspecified = AF_UNSPEC,
				Any = AF_UNSPEC
			};
		}

		const std::string NoSpecificPort = "", AllLocalInterfaces = "";

		std::string PortToString(unsigned short port) {
			return std::to_string(port);
		}

		struct AddressInfo {
			std::string IP;
			Hints::Protocol Protocol;
			Hints::Socket SocketType;
			Hints::Family Family;

			AddressInfo(std::string providedIP, int proto, int sock, int family) : IP(providedIP) {
				Family = Hints::revHintNum<Hints::Family>(family);
				Protocol = Hints::revHintNum<Hints::Protocol>(proto);
				SocketType = Hints::revHintNum<Hints::Socket>(sock);
			}

			AddressInfo() {
				IP.clear();
				Protocol = Hints::Protocol::Any;
				SocketType = Hints::Socket::Any;
				Family = Hints::Family::Any;
			}
		};

		/*
		* Resolves the hostname
		* Hostname can be a numeric address or a hostname (if Hints::Flags::OnlyNumericAddress isn't defined)
		* Port can be a numeric port or a service name (if Hints::Flags::OnlyNumericPort isn't defined)
		*/
		std::vector<AddressInfo> ResolveHostname(std::error_code* ec = nullptr, const std::string& hostname = AllLocalInterfaces, const std::string& port = NoSpecificPort, Hints::Flags flags = Hints::Flags::None, Hints::Protocol proto = Hints::Protocol::Any, Hints::Socket socket = Hints::Socket::Any, Hints::Family family = Hints::Family::Any) {
#ifdef _WIN32
			WSADATA wsaData;
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
				if (ec != nullptr) *ec = std::error_code(static_cast<int>(Error::ErrorCode::wSockFail), Error::getDNSErrorCategory());
				return {};
			}
#endif
			
			struct addrinfo hints = { 0 };
			struct addrinfo* ptr = 0;
			struct addrinfo* res = 0;

			memset(&hints, 0, sizeof(hints));

			hints.ai_family = Hints::hintNum(family);
			hints.ai_protocol = Hints::hintNum(proto);
			hints.ai_socktype = Hints::hintNum(socket);

			//hints.ai_flags = AI_CANONNAME;

			hints.ai_flags |= ((Hints::hintNum(flags) & Hints::hintNum(Hints::Flags::OnlyNumericAddress)) ? AI_NUMERICHOST : 0);

			hints.ai_flags |= ((Hints::hintNum(flags) & Hints::hintNum(Hints::Flags::OnlyNumericPort)) ? AI_NUMERICSERV : 0);

#ifndef _WIN32
			hints.ai_flags |= (Hints::hintNum(flags) & Hints::hintNum(Hints::Flags::UseIDNPunycode) ? AI_IDN : 0);
#endif
			{
				int ret;

				if ((ret = getaddrinfo(hostname.c_str(), (port.empty() ? 0 : port.c_str()), &hints, &res)) != 0) {
					if (ec != nullptr) *ec = std::error_code(ret, Error::getDNSErrorCategory());
					return {};
				}
			}

			std::vector<AddressInfo> resolvedList;

			for (ptr = res; ptr != 0; ptr = ptr->ai_next) {
				int ptrFamily = ptr->ai_family;
				bool isIPv4 = (ptrFamily == AF_INET);
				std::string resolvedIp((isIPv4 ? 16 : 46), '\00');

				auto getVersion = [isIPv4, &ptr]() -> void* {
					if (isIPv4) {
						return reinterpret_cast<void*>(&reinterpret_cast<struct sockaddr_in*>(ptr->ai_addr)->sin_addr);
					}
					else {
						return reinterpret_cast<void*>(&reinterpret_cast<struct sockaddr_in6*>(ptr->ai_addr)->sin6_addr);
					}
				};

				if (inet_ntop(ptrFamily, getVersion(), &resolvedIp[0], resolvedIp.size()) == 0) continue;

				resolvedList.emplace_back(std::move(resolvedIp), ptr->ai_protocol, ptr->ai_socktype, std::move(ptrFamily));
			}

			freeaddrinfo(res);

#ifdef _WIN32
			WSACleanup();
#endif

			return resolvedList;
		}
	}
}