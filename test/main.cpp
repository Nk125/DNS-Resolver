#include <DNS Resolver.hpp>
//#include "enum.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
	using namespace nk125;
	using namespace nk125::DNS::Hints;

	std::string hostname, service;
	
	if (argc >= 2) {
		hostname.assign(argv[1]);
		
	}
	else {
		std::cout << "Hostname not provided, falling back to AllLocalInterfaces\n";
		hostname = DNS::AllLocalInterfaces;
	}

	if (argc >= 3) {
		service.assign(argv[2]);
	}
	else {
		std::cout << "Service not provided, falling back to NoSpecificPort\n";
		service = DNS::NoSpecificPort;
	}

	std::cout << "Resolving: " << (hostname == DNS::AllLocalInterfaces ? "All Local Interfaces" : hostname) << "\n\n";

	std::error_code ec;

	for (nk125::DNS::AddressInfo& associatedIP : DNS::ResolveHostname(&ec, hostname, service, Flags::None, Protocol::Any, Socket::Any, Family::Any)) {
		std::cout << "IP: " << associatedIP.IP << "\n" <<
			"Family: " << (associatedIP.Family) << "\n" <<
			"Protocol: " << (associatedIP.Protocol) << "\n" <<
			"SocketType: " << (associatedIP.SocketType) << "\n\n";
	}

	if (ec) {
		std::cout << "Error: " << ec.message() << "\n";
	}
}
