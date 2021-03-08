#include "packet.hpp"

Packet::Packet(int frame, 
		struct pcap_pkthdr header, 
		unsigned char *data,
		unsigned char error_type,
		std::string prot) : frame(frame), 
	protocol(prot), 
	error_type(error_type) {


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
	this->tcp_header = (struct tcp_header *)(this->get_data() + 
			sizeof(struct ether_header) + 
			(unsigned int)(this->get_ip_header()->type_helen & 0xf));
}


PacketStream::PacketStream(unsigned char error_type) : error_type(error_type) {

}

PacketStream::~PacketStream() {
	for (unsigned int i = 0; i < this->packet_stream.size(); ++i) 
		delete this->packet_stream[i];
}

void PacketStream::push_back(int frame, 
		struct pcap_pkthdr header, 
		unsigned char *data) {

	struct ether_header *e_header = (struct ether_header *)(data);
	struct ip_header *i_header = (struct ip_header *)(data + sizeof(struct ether_header));

	Packet *pkt;
	if (htons(e_header->type) == ETHERTYPE_IP) {
		// Parse IP Header 
		// Ensure Type == 4 && HeLen == 5 otherwise we don't deal with typical data
		switch(i_header->protocol) {
			case ICMP:
				pkt = new ICMPPacket(frame, header, data, error_type);
				break;
			case TCP:
				pkt = new TCPPacket(frame, header, data, error_type);
				break;
			case UDP:
				pkt = new UDPPacket(frame, header, data, error_type);
				break;
			default:
				pkt = new IPPacket(frame, header, data, error_type);
				break;
		}

	} else if (htons(e_header->type) == ETHERTYPE_ARP) {
		pkt = new ARPPacket(frame, header, data, error_type);
	} else
		pkt = new Packet(frame, header, data, error_type);	// Create new unknown packet object

	this->packet_stream.push_back(pkt);	// Add pointer to the end of stream

	//printf("Type: %d\n", (i_header->type_helen & 0xf0)>>4);
	//printf("HeLen: %d\n", i_header->type_helen & 0x0f);
	//printf("Packet Length: %d or 0x%x\n",
	//        i_header->len,
	//        i_header->len);
	//printf("Time To Live: %d\n", i_header->ttl);
	//printf("Protocol: 0x%02x | %s\n", i_header->protocol, pkt->get_protocol().c_str());
	//printf("Checksum: 0x%04x\n", i_header->checksum);
	std::cout << "Source i: " << Utils::convert_ip(i_header->source) << std::endl;
	std::cout << "Dest   i: " << Utils::convert_ip(i_header->dest) << std::endl;
    //
	//std::cout << "Protocol: " << pkt->get_protocol() << std::endl;
}

