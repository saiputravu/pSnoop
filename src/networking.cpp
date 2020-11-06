#include "networking.hpp"
#include "Utils/utils.hpp"
#include "Utils/error.hpp"

Networking::Networking() {
	this->populate_interfaces();
}

Networking::~Networking() {
	// Free all the allocated memory, prevent a memory leak
	pcap_freealldevs(this->devices[0]);
	pcap_close(this->handle);
}

bool Networking::set_subnet_netmask() {
	if (pcap_lookupnet(this->selected_device,
				&this->subnet,
				&this->netmask,
				this->errbuf) == -1)  {
		Error::handle_error(this->errbuf, Error::CLI, "Networking::set_subnet_netmask");
		return false;
	}
	return true;
}

void Networking::open_live_device(int timeout, bool promiscuous) {
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

void Networking::setup_device(int index, int timeout, bool promiscuous) { 
	if (index >= 0 && index < (this->devices).size()) {
		this->selected_device = (this->devices[index])->name;
		this->set_subnet_netmask();	
		this->open_live_device(timeout, promiscuous);
	}
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

void Networking::start_listening(int max_count) {
	unsigned char *packet;
	struct pcap_pkthdr header;

	// Loop for every packet captured until conditions met 
	while ((true && max_count == 0) || this->packet_count < (unsigned int)max_count) {
		if (this->get_next_packet(&packet, &header) != 0) {
			Error::handle_error((char *)"Unable to capture packet.", Error::CLI);
			continue;
		}

		// Add packet to packet stream
		this->packet_stream->push_back(this->packet_count, header, packet);

		// If max_count is specified, log to stdout how many captured.
		if (max_count != 0)
			std::cout << "[" << this->packet_count << "/" << max_count << "] Packet Captured" << std::endl;

		// Separate output lines, commented out for now

		//std::cout << "\t[-] Length: " << packet_obj->get_header_len() << std::endl;
		//std::cout << "\t[-] Time  : " << Utils::convert_time(packet_obj->get_header_timestamp()) << std::endl;
		//
		//std::cout << std::endl;
		//Utils::hexdump(packet_obj->get_data(), packet_obj->get_header_len());
		//std::cout << std::endl;

		// Increment successful packet count
		this->packet_count++;
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

