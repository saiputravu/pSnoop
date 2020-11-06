#include <iostream>
#include <cstdio>
#include <thread>

#include "networking.hpp"
#include "Utils/utils.hpp"

int main() {

	// Select capture interface
	Networking capture;
	capture.print_interfaces();
	capture.setup_device(0);

	std::cout << "Selected device " << capture.get_cur_device() << std::endl;
	std::cout << "\tSubnet:  " << Utils::convert_bpf(capture.get_subnet()) << std::endl;
	std::cout << "\tNetmask: " << Utils::convert_bpf(capture.get_netmask()) << std::endl;
	
	// Setup and start listening (specify count to set how many to capture)
	capture.set_filter((const char *)"port 53");
	capture.start_listening(100);
	
	// Displaying packets
	for (int i = 0; i < 100; ++i) {
		Packet *pkt = (*(PacketStream *)capture.get_packet_stream())[i];
		if (pkt == 0) {
			printf("Error on packet [%d] [0x%x]\n", i, pkt);
			return -1;
		}
		std::cout << "[" << pkt->get_frame() << "] Packet Captured" << std::endl;
		std::cout << "\t[-] Length: " << pkt->get_header_len() << std::endl;
		std::cout << "\t[-] Time  : " << Utils::convert_time(pkt->get_header_timestamp()) << std::endl;

		std::cout << std::endl;
		Utils::hexdump(pkt->get_data(), pkt->get_header_len());
		std::cout << std::endl;
	}

	return 0;
}


