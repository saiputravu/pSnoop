#include <iostream>
#include <cstdio>

#include "networking.hpp"
#include "Utils/utils.hpp"

int main() {

	Networking capture;
	capture.print_interfaces();
	capture.set_cur_device(0);

	std::cout << "Selected device " << capture.get_cur_device() << std::endl;
	capture.start_listening();
	std::cout << "Started Listening for packets!" << std::endl;
	
	while (true) {
		char *packet;
		struct pcap_pkthdr hdr;

		capture.get_next_packet(&packet, &hdr);
		std::cout << "[*] Packet Captured" << std::endl;
		std::cout << "\t[-] Length: " << hdr.len << std::endl;
		std::cout << "\t[-] Time  : " << Utils::convert_time(hdr.ts.tv_sec) << std::endl;

		std::cout << std::endl;
		Utils::hexdump(packet, hdr.len);		
		std::cout << std::endl;
	}

	return 0;
}


