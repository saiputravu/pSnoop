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
	pcap_if_t *get_device(int index) { return devices[index]; }

	void print_interfaces();

private:
	int populate_interfaces();
	
	char errbuf[PCAP_ERRBUF_SIZE];
	std::vector<pcap_if_t *> devices;
};


#endif
