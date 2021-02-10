#ifndef PACKET_H
#define PACKET_H

#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/time.h>
#include <vector>
#include <arpa/inet.h>

#include "protocols.hpp"
#include "Utils/error.hpp"
#include "Utils/utils.hpp"

class Packet {
	public:
		Packet(int frame,
				struct pcap_pkthdr header,
				unsigned char *data,
				struct ether_header *e_header,
				struct ip_header *i_header,
				unsigned char error_type = Error::CLI,
				std::string prot = "UNKNOWN");
		virtual ~Packet();

		// Getters
		int get_frame() { return this->frame; }
		unsigned int get_header_len() { return this->header->len; }
		time_t get_header_timestamp() { return this->header->ts.tv_sec; }
		unsigned char *get_data() { return this->data; }
		unsigned char *get_packet() { return this->data; }
		std::string get_protocol() { return this->protocol; }
		std::string get_info() { return this->info; }
		struct ether_header *get_ether_header() { return this->ether_header; }
		struct ip_header *get_ip_header() { 
			if (this->ip_header)
				return this->ip_header;
			return nullptr;
		}

		// Setters

	private:
		// Methods
		virtual void parse();

		// Properties

		int frame = -1;							// Position packet received
		struct pcap_pkthdr *header = nullptr; 	// Pointer to packet header struct
		unsigned char *data = nullptr;			// Pointer to packet data 

		struct ether_header *ether_header;
		struct ip_header *ip_header;

		std::string protocol = "";				// Protocol Type
		std::string info = "";					// Extra information filled at instantiation
		unsigned char error_type;

};

class IPPacket : public Packet {
	public:

		IPPacket(int frame,
				struct pcap_pkthdr header,
				unsigned char *data, 
				struct ether_header *e_header,
				struct ip_header *i_header,
				unsigned char error_type = Error::CLI
				) : Packet(frame, header, data, e_header, i_header, error_type, "IP") {

		}

		~IPPacket() {} 

	private:
};

class ICMPPacket : public Packet {
	public:

		ICMPPacket(int frame,
				struct pcap_pkthdr header,
				unsigned char *data,
				struct ether_header *e_header,
				struct ip_header *i_header,
				unsigned char error_type = Error::CLI
				) : Packet(frame, header, data, e_header, i_header, error_type, "ICMP") {

		}

		~ICMPPacket() {} 

	private:
};

class TCPPacket : public Packet {
	public:

		TCPPacket(int frame,
				struct pcap_pkthdr header,
				unsigned char *data,
				struct ether_header *e_header,
				struct ip_header *i_header,
				unsigned char error_type = Error::CLI
				) : Packet(frame, header, data, e_header, i_header, error_type, "TCP") {

		}

		~TCPPacket() {} 

	private:
};

class UDPPacket : public Packet {
	public:

		UDPPacket(int frame,
				struct pcap_pkthdr header,
				unsigned char *data,
				struct ether_header *e_header,
				struct ip_header *i_header,
				unsigned char error_type = Error::CLI
				) : Packet(frame, header, data, e_header, i_header, error_type, "UDP") {

		}

		~UDPPacket() {} 

	private:
};

class ARPPacket : public Packet {
	public:

		ARPPacket(int frame,
				struct pcap_pkthdr header,
				unsigned char *data,
				struct ether_header *e_header,
				struct ip_header *i_header,
				unsigned char error_type = Error::CLI
				) : Packet(frame, header, data, e_header, i_header, error_type, "ARP") {

		}

		~ARPPacket() {} 

	private:
};

class PacketStream {
	public:
		PacketStream(unsigned char error_type = Error::CLI);
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
		unsigned char error_type;

};

#endif // PACKET_H
