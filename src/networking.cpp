#include "networking.hpp"
#include "error.hpp"

Networking::Networking() {
	this->populate_interfaces();
}

Networking::~Networking() {
	// Free all the allocated memory, prevent a memory leak
	pcap_freealldevs(this->devices[0]);
}

int Networking::populate_interfaces() {
	// Gather all devices
	pcap_if_t *temp_dev, *temp;
	if (pcap_findalldevs(&temp_dev, this->errbuf) == -1) {
		Error::handle_error(this->errbuf, Error::CLI);	
		return -1;
	}

	// Put devices into a vector (dynamic array), instead of using linked list
	for(temp = temp_dev; temp; temp = temp->next)
		(this->devices).push_back(temp);

	return 0;
}

void Networking::start_listening() {
	// Check the network interface string is selected
	if (this->open_device == NULL) {
		Error::handle_error((char *)"Please select a device to listen on", Error::CLI);
		return;
	}

	// Open in non-promisc mode, with timeout of unlimited
	this->handle = pcap_open_live(this->open_device, BUFSIZ, 0, 0, this->errbuf);
	if (this->handle == NULL) {
		Error::handle_error(this->errbuf, Error::CLI);
		return;
	}
}

void Networking::get_next_packet(char **packet, struct pcap_pkthdr *hdr) {
	// Get packet data, packet header and place it into the pointers 
	// given in arguments
	*packet = (char *)pcap_next(this->handle, hdr);
	if (*packet == NULL) {
		Error::handle_error(this->errbuf, Error::CLI);
		return;
	}
}

void Networking::print_interfaces() {
	// Output all interfaces to stdout
	std::cout << "Available devices list: " << std::endl;
	for (int i = 0; i < (this->devices).size(); ++i)
		std::cout << "\t[" << i << "] " << (this->devices)[i]->name << std::endl;
}
