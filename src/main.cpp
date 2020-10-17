#include <iostream>
#include <cstdio>
#include <thread>

#include "networking.hpp"
#include "Utils/utils.hpp"

int main() {

	Networking capture;
	capture.print_interfaces();
	capture.setup_device(0);

	std::cout << "Selected device " << capture.get_cur_device() << std::endl;
	std::cout << "\tSubnet:  " << Utils::convert_bpf(capture.get_subnet()) << std::endl;
	std::cout << "\tNetmask: " << Utils::convert_bpf(capture.get_netmask()) << std::endl;
	
	capture.set_filter((const char *)"port 53");
	capture.start_listening();
	
	std::cout << "Started Listening for packets!" << std::endl;
	
	return 0;
}


