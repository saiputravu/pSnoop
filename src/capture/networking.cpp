#include "networking.hpp"

Networking::Networking(unsigned char error_type) : error_type(error_type){
	this->packet_stream = new PacketStream(error_type);
	this->populate_interfaces();
}

Networking::~Networking() {
	// Free all the allocated memory, prevent a memory leak
	if (this->devices[0])
		pcap_freealldevs(this->devices[0]);
	
	if (this->handle)
		pcap_close(this->handle);
}

bool Networking::set_subnet_netmask() {
	if (pcap_lookupnet(this->selected_device,
				&this->subnet,
				&this->netmask,
				this->errbuf) == -1)  {
		Error::handle_error(this->errbuf, 
				this->error_type, "Networking::set_subnet_netmask");
		return false;
	}
	return true;
}

void Networking::open_live_device(int timeout, bool promiscuous) {
	// Check the network interface string is selected
	if (this->selected_device == NULL) {
		Error::handle_error((char *)"Please select a device to listen on", 
				this->error_type, "Networking::open_live_device");
		return;
	}

	// Open in (non-)promiscuous mode, with timeout of timeout 
	this->handle = pcap_open_live(this->selected_device, 
			BUFSIZ, 
			promiscuous, 
			timeout, 
			this->errbuf);
	if (this->handle == NULL) {
		Error::handle_error(this->errbuf, 
				this->error_type, "Networking::open_live_device");
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
		Error::handle_error(this->errbuf, 
				this->error_type, "Networking::populate_interfaces");	
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
			Error::handle_error((char *)"Unable to capture packet.", 
					this->error_type, "Networking::start_listening(int)");
			continue;
		}

		// Add packet to packet stream
		this->packet_stream->push_back(this->packet_count, header, packet);

		// If max_count is specified, log to stdout how many captured.
		if (max_count != 0)
			std::cout << "[" << this->packet_count << "/" << max_count << "] Packet Captured" << std::endl;

		// Increment successful packet count
		this->packet_count++;
	}
	return;
}

void Networking::start_listening(bool *active) {
	unsigned char *packet;
	struct pcap_pkthdr header;

	printf("Started Listening\n");
	// Loop for every packet captured until conditions met 
	while (*active) {
		if (this->get_next_packet(&packet, &header) != 0) {
			Error::handle_error((char *)"Unable to capture packet.", 
					this->error_type), "Networking::start_listening(bool &)";
			continue;
		}

		// Add packet to packet stream
		this->packet_stream->push_back(this->packet_count, header, packet);

		// Increment successful packet count
		this->packet_count++;
	}
	return;
}

int Networking::get_next_packet(unsigned char **packet, struct pcap_pkthdr *header) {
	// Get packet data, packet header and place it into the pointers 
	// given in arguments

	*packet = (unsigned char *)pcap_next(this->handle, header);
	if (*packet == NULL) {
		Error::handle_error(this->errbuf, 
				this->error_type, "Networking::get_next_packet");
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
		Error::handle_error((char *)"Couldn't parse filter expression.", 
				this->error_type, "Networking::set_filter");
		return -1;
	}

	if (pcap_setfilter(this->handle, &compiled_filter) == -1) {
		Error::handle_error((char *)"Couldn't install filter expression.", 
				this->error_type, "Networking::set_filter");
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
