#ifndef NETWORKING_H
#define NETWORKING_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <future>

#include <pcap.h>

#include "Utils/utils.hpp"
#include "Utils/error.hpp"

class Networking {
public:
	Networking();
	~Networking();

	// Getters
	pcap_t *get_handle() { return this->handle; }
	pcap_if_t *get_device(int index) { return this->devices[index]; }
	char *get_cur_device() { return this->selected_device; }
	bpf_u_int32 get_subnet() { return this->subnet; }
	bpf_u_int32 get_netmask() { return this->netmask; }
	unsigned int get_packet_count() { return this->packet_count; }

	// Setters
	bool set_subnet_netmask() {
		if (pcap_lookupnet(this->selected_device,
							&this->subnet,
							&this->netmask,
							this->errbuf) == -1)  {
			Error::handle_error(this->errbuf, Error::CLI, "Networking::set_subnet_netmask");
			return false;
		}
		return true;
	}

	void setup_device(int index, int timeout=1000, bool promiscuous=false) { 
		if (index >= 0 && index < (this->devices).size()) {
			this->selected_device = (this->devices[index])->name;
			this->set_subnet_netmask();	
			this->open_live_device(timeout, promiscuous);
		}
	}
	
	// Methods
	void start_listening(int count=0);
	int get_next_packet(unsigned char **packet, struct pcap_pkthdr *header);
	int set_filter(const char *expression, int optimize=0);

	// Miscellaneous
	void print_interfaces();

private:
	// Methods
	int populate_interfaces();
	
	void open_live_device(int timeout, bool promiscuous) {
		// Check the network interface string is selected
		if (this->selected_device == NULL) {
			Error::handle_error((char *)"Please select a device to listen on", Error::CLI);
			return;
		}

		// Open in (non-)promiscuous mode, with timeout of timeout 
		this->handle = pcap_open_live(this->selected_device, 
										BUFSIZ, 
										promiscuous, 
										timeout, 
										this->errbuf);
		if (this->handle == NULL) {
			Error::handle_error(this->errbuf, Error::CLI);
			return;
		}
	}

	// Properties
	char errbuf[PCAP_ERRBUF_SIZE];		// Error buffer, holds last error

	pcap_t *handle;						// pcap_t handle to the selected network interface
	std::vector<pcap_if_t *> devices;	// Current devices available on the computer

	char *selected_device;		// String of the selected network interface
	bpf_u_int32 subnet = 0;		// 32bit IP Subnet of selected network interface
	bpf_u_int32 netmask = 0;	// 32bit Network Mask of the selected network interface
	
	unsigned int packet_count = 0;
};

#endif
