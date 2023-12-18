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

```cpp
using namespace nk125::DNS;

// This will show up all IPs registered to that domain
for (AddressInfo& addressInfo : ResolveHostname(nullptr, "example.com")) {
	std::cout << "IP Address: " addressInfo.IP << "\n";
}
```