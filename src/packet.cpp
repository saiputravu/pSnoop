#include "packet.hpp"
#include "Utils/error.hpp"
#include "Utils/utils.hpp"

Packet::Packet(int frame, 
		struct pcap_pkthdr header, 
		unsigned char *data) {

	this->frame = frame;	// Set frame as given number

	// Allocate heap space for header struct and set address as data
	this->header = (struct pcap_pkthdr *)calloc(1, sizeof(struct pcap_pkthdr));
	if (this->header == nullptr)
		Error::handle_error((char *)"Error in allocating heap space for packet header.", Error::CLI, "Packet::Packet");
	else
		*(this->header) = header;

	// Allocate heap space for packet data and copy data into it
	this->data = (unsigned char *)calloc(this->header->len, sizeof(char));
	if (this->header == nullptr)
		Error::handle_error((char *)"Error in allocating heap space for packet data.", Error::CLI, "Packet::Packet");
	else {
		for (unsigned int i = 0; i < this->header->len; ++i)
			this->data[i] = data[i];
	}

	// Parse the protocol heirarchy
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
	this->ether_header = (struct ether_header *)(this->data);

	if (htons(this->ether_header->type) == ETHERTYPE_IP) {
		// Parse IP Header 
		// Ensure Type == 4 && HeLen == 5 otherwise we don't deal with typical data
		this->ip_header = (struct ip_header *)(this->data + sizeof(struct ether_header));

		switch(this->ip_header->protocol) {
			case ICMP:
				this->protocol = "ICMP";
				break;
			case IPv4:
				this->protocol = "IPv4";
				break;
			case TCP:
				this->protocol = "TCP";
				break;
			case UDP:
				this->protocol = "UDP";
				break;
			default:
				this->protocol = "IP";
				break;
		}
		printf("Type: %d\n", (this->ip_header->type_helen & 0xf0)>>4);
		printf("HeLen: %d\n", this->ip_header->type_helen & 0x0f);
		printf("Packet Length: %d or 0x%x\n", 
				htons(this->ip_header->len),
				htons(this->ip_header->len));
		printf("Time To Live: %d\n", this->ip_header->ttl);
		printf("Protocol: 0x%02x | %s\n", this->ip_header->protocol, this->protocol.c_str());
		printf("Checksum: 0x%04x\n", htons(this->ip_header->checksum));
		std::cout << "Source IP: " << Utils::convert_ip(htonl(this->ip_header->source)) << std::endl;
		std::cout << "Dest   IP: " << Utils::convert_ip(htonl(this->ip_header->dest)) << std::endl;

	} else if (htons(this->ether_header->type) == ETHERTYPE_ARP) {
		this->protocol = "ARP";
	} else {
		this->protocol = "UNKNOWN";
		this->info = "Unknown packet protocol";
	}
}


PacketStream::PacketStream() {

}

PacketStream::~PacketStream() {

}

void PacketStream::push_back(int frame, 
		struct pcap_pkthdr header, 
		unsigned char *data) {

	Packet *pkt = new Packet(frame, header, data);	// Create new packet object
	this->packet_stream.push_back(pkt);				// Add pointer to the end of stream
}

