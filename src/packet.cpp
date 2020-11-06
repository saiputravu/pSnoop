#include "packet.hpp"
#include "Utils/error.hpp"

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
}

Packet::~Packet() {
	// Free heap data before deletion
	if (this->header != nullptr)
		free(this->header);
	if (this->data != nullptr)
		free(this->data);
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

