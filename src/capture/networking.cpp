#include "networking.hpp"

Networking::Networking(unsigned char error_type) : error_type(error_type){
	this->packet_stream = new PacketStream(error_type);
	// Open a dump file

	this->populate_interfaces();
}

Networking::~Networking() {
	// Free all the allocated memory, prevent a memory leak
	if (this->devices[0])
		pcap_freealldevs(this->devices[0]);


	// Just delete temp file as dumper is automatically free'd
	if (this->dumper)
		this->close_dump_without_save();

	if (this->handle)
		pcap_close(this->handle);
}

void Networking::create_new_dump() {
	this->tempfile = std::tmpnam(nullptr);
	this->dumper = pcap_dump_open(this->handle, this->tempfile.c_str());
	if (this->dumper == nullptr) {
		Error::handle_error("Unable to create pcap dumper",
				this->error_type, "Networking::create_new_dump");
		return;
	}
}

void Networking::close_dump(std::string savefile) {
	// Ensure savefile is full path otherwise tmp file moved to current dir.
	
	if (this->dumper) {
		// Delete temp file
		pcap_dump_close(this->dumper);

		// Move temp file to savefile location
		// Find a better way than literally copying data file to file
		// Which is also OS independant
		std::ifstream in(this->tempfile, std::ios::in | std::ios::binary);
		std::ofstream out(savefile, std::ios::out | std::ios::binary);
		out << in.rdbuf();
		std::remove(this->tempfile.c_str());
	}
}

void Networking::close_dump_without_save() {
	// Only delete file, dumper is auto-free'd
	if (this->dumper) {
		std::ifstream f(this->tempfile.c_str());
		if (f.good())
			std::remove(this->tempfile.c_str());
	}
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

	this->create_new_dump();
	this->select_file = false;
	this->select_interface = true;
}

void Networking::open_offline_device(const char *filename) {
	// Check if file exists
	std::ifstream f(filename);
    if (!f.good()) {
		Error::handle_error((char *)"Please enter a proper file / filepath",
				this->error_type, "Networking::open_offline_device");
		return;
	}

	// Open offline mode 
	this->handle = pcap_open_offline(filename, this->errbuf);
	if (this->handle == NULL) {
		Error::handle_error(this->errbuf, 
				this->error_type, "Networking::open_live_device");
		return;
	}

	this->create_new_dump();
	this->select_file = true;
	this->select_interface = false;
}

void Networking::open_file(const char *filename) {

	// Set class attribute filename 
	strncpy(this->selected_file, filename, 256);
	this->selected_file[255] = 0x00;

	this->open_offline_device(filename);
}

void Networking::setup_device(int index, int timeout, bool promiscuous) { 
	if (index >= 0 && index < (this->devices).size()) {
		strncpy(this->selected_device, (this->devices[index])->name, 256);
		this->selected_device[255] = 0x00;

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

			// This is useful for detecting EOF for .pcap files
			if (this->select_file)
				this->capture_packet_err_count++;
			if (this->capture_packet_err_count > 10)
				max_count = 1;
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

	// Loop for every packet captured until conditions met
	do {
		if (this->get_next_packet(&packet, &header) != 0) {
			Error::handle_error((char *)"Unable to capture packet.", 
				this->error_type, "Networking::start_listening(bool &)");
			if (this->select_file)
				this->capture_packet_err_count++;
			if (this->capture_packet_err_count > 10)
				*active = false;
			continue;
		}

		// Add packet to packet stream
		this->packet_stream->push_back(this->packet_count, header, packet);
		emit this->packet_recv((*this->packet_stream)[this->packet_count]); // Emit SIGNAL to say I've received new packet

		// Increment successful packet count
		this->packet_count++;

	} while (*active);
	emit this->stopped_recv();
}

void Networking::save_packets_to_pcap(std::string filename) {
	if (this->packet_stream->size() <= 0)
		return;
	std::ofstream stream;
	stream.open(filename, std::ofstream::binary);
	if (!stream.is_open()) {
		Error::handle_error((char *)"Unable to open file to save .pcap", 
				this->error_type, "Networking::save_packets_to_pcap");
		return;
	}

	stream.write((char *)&this->pcap_hdr, sizeof(this->pcap_hdr));

	// Go through each packet, and write header:packet_data
	for (int i = 0; i < this->packet_stream->size(); ++i) {
		Packet *pkt = (*this->packet_stream)[i];
		stream.write((char *)pkt->get_packet_header(), sizeof(struct pcap_pkthdr));
		stream.write((char *)pkt->get_data(), pkt->get_header_len());
	}

	stream.close();
}

int Networking::listen_next_packet() {
	unsigned char *packet;
	struct pcap_pkthdr header;
	if (this->get_next_packet(&packet, &header) != 0) {
		if (this->capture_packet_err_count > 10)
			return -2;
		if (this->capture_packet_err_count == 0 || this->select_interface) 
			// Print when they aren't consectuive or a pcap file isn't selected
			Error::handle_error((char *)"Unable to capture packet.",
				this->error_type, "Networking::start_listening_to_loop()");
		if (this->select_file)
			this->capture_packet_err_count++;
		return -1;
	}

	// Add packet to packet stream
	this->packet_stream->push_back(this->packet_count, header, packet);
	emit this->packet_recv((*this->packet_stream)[this->packet_count]);

	this->packet_count++;
	if (this->select_file)
		this->capture_packet_err_count = 0; // Reset if the errors aren't consecutive
	return 0;
}

int Networking::get_next_packet(unsigned char **packet, struct pcap_pkthdr *header) {
	// Get packet data, packet header and place it into the pointers 
	// given in arguments

	*packet = (unsigned char *)pcap_next(this->handle, header);
	if (*packet == NULL) {
			return -2;
		if (this->capture_packet_err_count == 0 || this->select_interface) 
			// Print when they aren't consectuive or a pcap file isn't selected
			Error::handle_error(this->errbuf, 
				this->error_type, "Networking::get_next_packet");
		return -1;
	}

	// Save to packet dump file
	pcap_dump((unsigned char *)this->dumper, header, (const unsigned char *)*packet);

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

