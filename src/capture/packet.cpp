#include "packet.hpp"

Packet::Packet(int frame, 
		struct pcap_pkthdr header, 
		unsigned char *data,
		unsigned char error_type,
		std::string prot, 
		std::string fgcolour,
		std::string bgcolour
		) : frame(frame), 
	protocol(prot), 
	error_type(error_type),
	fgcolour(fgcolour), bgcolour(bgcolour) {


	// Allocate heap space for header struct and set address as data
	this->header = (struct pcap_pkthdr *)calloc(1, sizeof(struct pcap_pkthdr));
	if (this->header == nullptr)
		Error::handle_error((char *)"Error in allocating heap space for packet header.", this->error_type, "Packet::Packet");
	else
		*(this->header) = header;

	// Allocate heap space for packet data and copy data into it
	this->data = (unsigned char *)calloc(this->header->len, sizeof(char));
	if (this->header == nullptr)
		Error::handle_error((char *)"Error in allocating heap space for packet data.", this->error_type, "Packet::Packet");
	else {
		for (unsigned int i = 0; i < this->header->len; ++i)
			this->data[i] = data[i];
	}

	// Parse ip header, ether header 
	this->ether_header = (struct ether_header*)(this->data);
	this->ip_header = (struct ip_header*)(this->data + sizeof(struct ether_header));

	// Parse the data
	this->parse();

	// Setup tree structure 
	this->setup_tree();
}

Packet::~Packet() {
	// Free heap data before deletion
	if (this->header != nullptr)
		free(this->header);
	if (this->data != nullptr)
		free(this->data);
}

void Packet::parse() {
}

void TCPPacket::parse() {
	// Ether_header --> Ip_Header --> Tcp_Header
	// +sizeof()      +helen*32bits
	this->tcp_header = (struct tcp_header *)(this->get_data() + 
			sizeof(struct ether_header) + 
			4*(unsigned int)(this->get_ip_header()->type_helen & 0xf));

	// Set any general information 
	std::string information = this->get_info();
	information += "SrcPort: [";
	information += std::to_string(htons(this->tcp_header->source_port));
	information += "]";

	information += " | ";
	
	information += "DstPort: [";
	information += std::to_string(htons(this->tcp_header->dest_port));
	information +=  "]";

	information += " | ";

	// TCP Flags 
	unsigned short control = htons(this->tcp_header->offset_resv_control);
	if (control & 0b100000)
		information += "URG ";
	if (control & 0b010000)
		information += "ACK ";
	if (control & 0b001000)
		information += "PSH ";
	if (control & 0b000100)
		information += "RST ";
	if (control & 0b000010)
		information += "SYN ";
	if (control & 0b000001)
		information += "FIN ";

	this->set_info(information);
}	

void HTTPPacket::parse() {
	// Tcp_header + data offset * 4
	this->http_payload = (unsigned char *)(this->get_data() + 
			sizeof(struct ether_header) + 
			4*(unsigned int)(this->get_ip_header()->type_helen & 0xf) + 
			4*(unsigned int)(htons(this->get_tcp_header()->offset_resv_control)>>12));
}

void Packet::setup_tree() {
	// Setup what is printed in the tree 
	this->tree = new TreeStructure();
	unsigned int root_index;

	// Ether header
	this->tree->add_root_node("Ether Header", "");

	root_index = this->tree->get_tree_root_count() - 1;
	this->tree->add_child_node({root_index}, 
			"Source MAC Address", 
			Utils::convert_mac_address(this->get_ether_header()->shost));
	this->tree->add_child_node({root_index}, 
			"Destination MAC Address", 
			Utils::convert_mac_address(this->get_ether_header()->dhost));

	// Hex version of the string
	this->tree->add_child_node({root_index}, 
			"Ether Type", 
			Utils::to_hex((int)htons(this->get_ether_header()->type), 4));

	// IP header
	this->tree->add_root_node("IP Header", "");

	root_index = this->tree->get_tree_root_count() - 1;

	this->tree->add_child_node({root_index}, 
			"Source IP", 
			Utils::convert_ip(htonl(this->get_ip_header()->source)));
	this->tree->add_child_node({root_index}, 
			"Destination IP", 
			Utils::convert_ip(htonl(this->get_ip_header()->dest)));

	this->tree->add_child_node({root_index}, 
			"IP Version", 
			Utils::to_hex((this->get_ip_header()->type_helen&0xf0) >> 4, 1));
	this->tree->add_child_node({root_index}, 
			"Header Length (32 Bits)", 
			Utils::to_hex(this->get_ip_header()->type_helen&0x0f, 1));
	this->tree->add_child_node({root_index}, 
			"Type Of Service", 
			Utils::to_hex(this->get_ip_header()->tos, 2));
	this->tree->add_child_node({root_index}, 
			"Rest of packet length",
			Utils::to_hex(htons(this->get_ip_header()->len), 4));
	this->tree->add_child_node({root_index}, 
			"Packet ID",
			std::to_string((int)htons(this->get_ip_header()->id)));
	this->tree->add_child_node({root_index}, 
			"IP Flag", 
			Utils::to_info_binary(htons(this->get_ip_header()->flag_frag_offset), 16, 0xe000));
	this->tree->add_child_node({root_index}, 
			"Fragment Offset", 
			Utils::to_info_binary(htons(this->get_ip_header()->flag_frag_offset), 16, 0x1fff));
	this->tree->add_child_node({root_index}, 
			"Time to live (s)",
			std::to_string((int)this->get_ip_header()->ttl));
	this->tree->add_child_node({root_index}, 
			"Protocol Type",
			Utils::to_hex(this->get_ip_header()->protocol, 2));
	this->tree->add_child_node({root_index}, 
			"Checksum",
			Utils::to_hex(htons(this->get_ip_header()->checksum), 4));
}

void TCPPacket::setup_tree() {
	unsigned int root_index;

	// TCP header
	this->get_tree()->add_root_node("TCP Header", "");

	root_index = this->get_tree()->get_tree_root_count() - 1;
	this->get_tree()->add_child_node({root_index}, 
			"Source port", 
			std::to_string((int)htons(this->get_tcp_header()->source_port)));
	this->get_tree()->add_child_node({root_index},
			"Destination port",
			std::to_string((int)htons(this->get_tcp_header()->dest_port)));
	
	this->get_tree()->add_child_node({root_index}, 
			"Sequence number",
			Utils::to_hex(htonl(this->get_tcp_header()->seq), 8));
	this->get_tree()->add_child_node({root_index}, 
			"Acknowledgement number",
			Utils::to_hex(htonl(this->get_tcp_header()->ack), 8));

	this->get_tree()->add_child_node({root_index}, 
			"Data offset (32 Bits)",
			Utils::to_info_binary(htons(this->get_tcp_header()->offset_resv_control), 16, 0xf000));
	
	this->get_tree()->add_child_node({root_index}, 
			"Reserved",
			Utils::to_info_binary(htons(this->get_tcp_header()->offset_resv_control), 16, 0x0fc0));

	this->get_tree()->add_child_node({root_index}, 
			"TCP Flags",
			Utils::to_info_binary(htons(this->get_tcp_header()->offset_resv_control), 16, 0x003f) + " | URG,ACK,PSH,RST,SYN,FIN");

	this->get_tree()->add_child_node({root_index},
			"Window (bytes)",
			Utils::to_hex(htons(this->get_tcp_header()->window), 4));
	
	this->get_tree()->add_child_node({root_index},
			"Checksum",
			Utils::to_hex(htons(this->get_tcp_header()->checksum), 4));

	this->get_tree()->add_child_node({root_index},
			"Urgent Pointer",
			Utils::to_hex(htons(this->get_tcp_header()->urgent_pointer), 4));
}

void HTTPPacket::setup_tree() {
	unsigned int root_index;

	// HTTP header
	this->get_tree()->add_root_node("HTTP Header", "");

	root_index = this->get_tree()->get_tree_root_count() - 1;

	// Get the first line as display
	std::string payload = std::string((char *)this->http_payload);
	std::string first_line = payload.substr(0, payload.find("\n"));
	this->get_tree()->add_child_node({root_index}, 
			"HTTP Payload", 
			first_line + " ...");
}

PacketStream::PacketStream(unsigned char error_type) : error_type(error_type) {

}

PacketStream::~PacketStream() {
	for (unsigned int i = 0; i < this->packet_stream.size(); ++i) 
		delete this->packet_stream[i];
}

template <class T>
T *PacketStream::new_packet(int frame, 
		struct pcap_pkthdr header, 
		unsigned char *data, 
		std::string protocol) {

	// Protocol is expected to be in uppercase

	if (this->config->find(protocol + "_fg") == this->config->end() || 
		this->config->find(protocol + "_bg") == this->config->end()) 
		return new T(frame, header, data, this->error_type, protocol); 
	else
		return new T(frame, header, data, this->error_type, protocol,
			(*this->config)[protocol + "_fg"], (*this->config)[protocol + "_bg"]);
}

Packet *PacketStream::parse_tcp_packet(int frame, 
		struct pcap_pkthdr header, 
		unsigned char *data) { 
	
	struct ip_header *i_header = (struct ip_header *)(data + sizeof(struct ether_header));
	struct tcp_header *t_header = (struct tcp_header *)(data + sizeof(struct ether_header) +
			4*(unsigned int)(i_header->type_helen & 0x0f));

	unsigned int payload_offset = (unsigned int)(sizeof(struct ether_header) + 
				4*(unsigned int)(i_header->type_helen&0x0f) +
				4*(unsigned int)(htons(t_header->offset_resv_control) >> 12));

	if (header.len == payload_offset)
		// TCP packet has no data
		return this->new_packet<TCPPacket>(frame, header, data, "TCP");

	// TCP packet payload - application level protocols
	char *payload = (char *)(data + payload_offset);

	// Source port or destination port to detect protocol 
	if (strncmp("HTTP", payload, 4) == 0 || strncmp("GET", payload, 3) == 0 || strncmp("POST", payload, 4) == 0 || strncmp("PUT", payload, 3) == 0 || strncmp("DELETE", payload, 6) == 0) {
		// HTTP protocol 
		// Matches false positives for some reason, FIX
		return this->new_packet<HTTPPacket>(frame, header, data, "HTTP");
	} else if (htons(t_header->source_port) == 443 || htons(t_header->dest_port) == 443) {
		// HTTPS protocol
	}

	// No known TCP protocols matched
	return this->new_packet<TCPPacket>(frame, header, data, "TCP");
}

void PacketStream::push_back(int frame, 
		struct pcap_pkthdr header, 
		unsigned char *data) {

	struct ether_header *e_header = (struct ether_header *)(data);
	struct ip_header *i_header = (struct ip_header *)(data + sizeof(struct ether_header));

	Packet *pkt;
	if (htons(e_header->type) == ETHERTYPE_IP) {
		// Parse IP Header 
		// Ensure Type == 4 otherwise we don't deal with typical data
		// If the settings are given for fg and bg colours, use them
		switch(i_header->protocol) {
			case ICMP:
				pkt = this->new_packet<ICMPPacket>(frame, header, data, "ICMP");
				break;
			case TCP:
				pkt = this->parse_tcp_packet(frame, header, data); // Separately parse TCP protocols
				break;
			case UDP:
				pkt = this->new_packet<UDPPacket>(frame, header, data, "UDP");
				break;
			default:
				pkt = this->new_packet<IPPacket>(frame, header, data, "IP");
				break;
		}

	} else if (htons(e_header->type) == ETHERTYPE_ARP) {
		pkt = this->new_packet<ARPPacket>(frame, header, data, "ARP");
	} else
		pkt = this->new_packet<Packet>(frame, header, data, "UNKNOWN");	// Create new unknown packet object

	this->packet_stream.push_back(pkt);	// Add pointer to the end of stream

	//printf("Type: %d\n", (i_header->type_helen & 0xf0)>>4);
	//printf("HeLen: %d\n", i_header->type_helen & 0x0f);
	//printf("Packet Length: %d or 0x%x\n",
	//        i_header->len,
	//        i_header->len);
	//printf("Time To Live: %d\n", i_header->ttl);
	//printf("Protocol: 0x%02x | %s\n", i_header->protocol, pkt->get_protocol().c_str());
	//printf("Checksum: 0x%04x\n", i_header->checksum);
	//std::cout << "Source : " << Utils::convert_ip(i_header->source) << std::endl;
	//std::cout << "Dest   : " << Utils::convert_ip(i_header->dest) << std::endl;
    //
	//std::cout << "Protocol: " << pkt->get_protocol() << std::endl;
}

