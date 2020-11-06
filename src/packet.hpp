#ifndef PACKET_H
#define PACKET_H

#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <vector>


class Packet {
	public:
		Packet(int frame,
				struct pcap_pkthdr header,
				unsigned char *data);
		~Packet();

		// Getters
		int get_frame() { return this->frame; }
		unsigned int get_header_len() { return this->header->len; }
		time_t get_header_timestamp() { return this->header->ts.tv_sec; }
		unsigned char *get_data() { return this->data; }
		unsigned char *get_packet() { return this->data; }

		// Setters

	private:
		// Properties

		int frame = -1;							// Position packet received
		struct pcap_pkthdr *header = nullptr; 	// Pointer to packet header struct
		unsigned char *data = nullptr;			// Pointer to packet data 

};

class PacketStream {
	public:
		PacketStream();
		~PacketStream();

		// Getters
		Packet* operator[] (int index) { 
			if (index >= 0 && (unsigned int)index < this->packet_stream.size()) 
				return this->packet_stream[index]; 
			return nullptr;
		} 

		// Setters
		void push_back(int frame,
				struct pcap_pkthdr header,
				unsigned char *data);

	private:
		// Properties 

		std::vector<Packet *> packet_stream;	// List of captured packet objects 

};

#endif
