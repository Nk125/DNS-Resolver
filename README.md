## DNS Resolver

C++ getaddrinfo() wrapper

### Adding to your project

It's quite simple, just download and add to your include path, or using cmake:

```cmake
include(FetchContent)

# Downloads the library
FetchContent_Declare(dnsresolver
	GIT_REPOSITORY "https://github.com/Nk125/DNS-Resolver"
	GIT_TAG "master"
	GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(dnsresolver)

# Includes it into your project
target_link_libraries(YourProject PRIVATE DNS::Resolver)
```

### How to use

Quite straightforward, with a simple call you can start using the lib

Most simple implementation:

```cpp
using namespace nk125::DNS;

// This will show up all IPs registered to that domain
for (AddressInfo& addressInfo : ResolveHostname(nullptr, "example.com")) {
	std::cout << "IP Address: " << addressInfo.IP << "\n";
}
// If not then an error ocurred
```

With error handling:

```cpp
using namespace nk125::DNS;

std::error_code ec;

// This will show up all IPs registered to that domain
for (AddressInfo& addressInfo : ResolveHostname(&ec, "example.com")) {
	std::cout << "IP Address: " << addressInfo.IP << "\n";
}

std::cout << "Error: " << ec.message() << "\n";
```

With certain service/port:

```cpp
using namespace nk125::DNS;

std::error_code ec;

// Can be a service like "http", "ftp", etc., or a port like "80", "22"
// To stringify ports you can use DNS::PortToString(80)
for (AddressInfo& addressInfo : ResolveHostname(&ec, "example.com", "http")) {
	std::cout << "IP Address: " << addressInfo.IP << "\n";

	// Socket type: 1 (DNS::Hints::SocketType::Stream)
	std::cout << "Socket type: " << static_cast<int>(addressInfo.SocketType) << "\n";
}

std::cout << "Error: " << ec.message() << "\n";
```

With filters:

```cpp
using namespace nk125::DNS;

// This will show up all IPs registered to that domain
// Also this implementation can be combined with error handling
for (AddressInfo& addressInfo : ResolveHostname(nullptr, "example.com", NoSpecificPort, Hints::Flags::UseIDNPunycode)) {
	std::cout << "IP Address: " << addressInfo.IP << "\n";
}
```

Also you can handle IPv4 and IPv6 IP addresses:

```cpp
using namespace nk125::DNS;

for (AddressInfo& addressInfo : ResolveHostname(nullptr, "::1" /*or "127.0.0.1"*/)) {
	std::cout << "IP Address: " << addressInfo.IP << "\n";

	// Family: IP version 6

	std::cout << "Family: ";

	if (addressInfo.Family == Hints::Family::IPv6) {
		std::cout << "IP version 6\n";
	}
	else if (addressInfo.Family == Hints::Family::IPv4) {
		std::cout << "IP version 4\n";
	}
	else {
		std::cout << "Unspecified\n";
	}
}
```

## Contribution

You're free to add any contribution through a pull request!
