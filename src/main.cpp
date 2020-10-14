#include <iostream>
#include <cstdio>

#include "networking.hpp"

int main() {

	Networking capture;
	capture.print_interfaces();
	capture.set_cur_device(0);

	std::cout << "Selected device [0] " << capture.get_cur_device() << std::endl;
	capture.start_listening();
	
	while (true) {
		char *packet;
		struct pcap_pkthdr hdr;

		capture.get_next_packet(&packet, &hdr);
		std::cout << "[*] Packet Captured" << std::endl;
		std::cout << "\t[-] Length: " << hdr.len << std::endl;
		std::cout << "\t[-] Time  : " << ctime((const time_t*)&hdr.ts.tv_sec) << std::endl;
	}



	return 0;
}


