#ifndef NETWORKING_H
#define NETWORKING_H

#include <iostream>
#include <cstdio>
#include <vector>

#include <pcap.h>

class Networking {
public:
	Networking();
	~Networking();

	// Setters and Getters
	pcap_if_t *get_device(int index) { return this->devices[index]; }
	char *get_cur_device() { return this->open_device; }
	void set_cur_device(int index) { 
		if (index >= 0 && index < (this->devices).size()) 
			this->open_device = (this->devices[index])->name; 
	}

	void start_listening();
	void get_next_packet(char **packet, struct pcap_pkthdr *hdr);
	void print_interfaces();

private:
	int populate_interfaces();
	
	char errbuf[PCAP_ERRBUF_SIZE];
	std::vector<pcap_if_t *> devices;
	char *open_device;
	pcap_t *handle;
		
};


#endif
