#include "networking.hpp"

Networking::Networking() {
	this->populate_interfaces();
}

Networking::~Networking() {
	// Free all the allocated memory, prevent a memory leak
	pcap_freealldevs(this->devices[0]);
	pcap_close(this->handle);
}

int Networking::populate_interfaces() {
	// Gather all devices
	pcap_if_t *temp_dev, *temp;
	if (pcap_findalldevs(&temp_dev, this->errbuf) == -1) {
		Error::handle_error(this->errbuf, Error::CLI, "Networking::populate_interfaces");	
		return -1;
	}

	// Put devices into a vector (dynamic array), instead of using linked list
	for(temp = temp_dev; temp; temp = temp->next)
		(this->devices).push_back(temp);

	return 0;
}

void Networking::start_listening(int count) {
	unsigned char *packet;
	struct pcap_pkthdr header;

	// Loop for every packet captured until conditions met 
	while (true || (count != 0 && this->packet_count < count)) {
		if (this->get_next_packet(&packet, &header) != 0) {
			// Error::handle_error((char *)"Unable to capture packet.", Error::CLI);
			continue;
		}

		std::cout << "[" << this->packet_count << "] Packet Captured" << std::endl;
		std::cout << "\t[-] Length: " << header.len << std::endl;
		std::cout << "\t[-] Time  : " << Utils::convert_time(header.ts.tv_sec) << std::endl;

		std::cout << std::endl;
		Utils::hexdump(packet, header.len);
		std::cout << std::endl;
	}
}

int Networking::get_next_packet(unsigned char **packet, struct pcap_pkthdr *header) {
	// Get packet data, packet header and place it into the pointers 
	// given in arguments
	
	*packet = (unsigned char *)pcap_next(this->handle, header);
	if (*packet == NULL) {
		Error::handle_error(this->errbuf, Error::CLI, "Networking::get_next_packet");
		return -1;
	}

	// Increment successful packet count
	this->packet_count++;
	return 0;
}

int Networking::set_filter(const char *expression, int optimize) {
	struct bpf_program compiled_filter;
	if (pcap_compile(this->handle,
				&compiled_filter, 
				expression, 
				optimize, 
				this->netmask) == -1) {
		Error::handle_error((char *)"Couldn't parse filter expression.", Error::CLI);
		return -1;
	}
	
	if (pcap_setfilter(this->handle, &compiled_filter) == -1) {
		Error::handle_error((char *)"Couldn't install filter expression.", Error::CLI);
		return -2;
	}
	return 0;
}

void Networking::print_interfaces() {
	// Output all interfaces to stdout
	std::cout << "Available devices list: " << std::endl;
	for (int i = 0; i < (this->devices).size(); ++i)
		std::cout << "\t[" << i << "] " << (this->devices)[i]->name << std::endl;
}

