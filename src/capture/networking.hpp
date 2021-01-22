#ifndef NETWORKING_H
#define NETWORKING_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <future>

#include <pcap.h>

#include "packet.hpp"

class Networking {
	public:
		Networking();
		~Networking();

		// Getters
		pcap_t *get_handle() { return this->handle; }
		pcap_if_t *get_device(int index) { 
			if (index >= 0 && index < (int) this->devices.size())
				return (this->devices)[index];
			return nullptr;
		}
		char *get_cur_device() { return this->selected_device; }
		bpf_u_int32 get_subnet() { return this->subnet; }
		bpf_u_int32 get_netmask() { return this->netmask; }
		unsigned int get_packet_count() { return this->packet_count; }
		PacketStream *get_packet_stream() { return this->packet_stream; }

		// Setters
		bool set_subnet_netmask();
		void setup_device(int index, int timeout=1000, bool promiscuous=false); 

		// Methods
		void start_listening(int max_count=0);
		int get_next_packet(unsigned char **packet, struct pcap_pkthdr *header);
		int set_filter(const char *expression, int optimize=0);

		// Miscellaneous
		void print_interfaces();

	private:
		// Methods
		int populate_interfaces();
		void open_live_device(int timeout, bool promiscuous);

		// Properties
		char errbuf[PCAP_ERRBUF_SIZE];		// Error buffer, holds last error

		pcap_t *handle = nullptr;			// pcap_t handle to the selected network interface
		std::vector<pcap_if_t *> devices;	// Current devices available on the computer

		char *selected_device;		// String of the selected network interface
		bpf_u_int32 subnet = 0;		// 32bit IP Subnet of selected network interface
		bpf_u_int32 netmask = 0;	// 32bit Network Mask of the selected network interface

		unsigned int packet_count = 0;						// Count of packets captured so far
		PacketStream *packet_stream = new PacketStream();	// Stream / List of all captured packet objects
};

#endif // NETWORKING_H
