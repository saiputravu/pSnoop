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
				unsigned char error_type = Error::CLI,
				std::string prot = "UNKNOWN",
				std::string fgcolour = "",
				std::string bgcolour = "");
		virtual ~Packet();

		// Getters
		int get_frame() { return this->frame; }
		unsigned int get_header_len() { return this->header->len; } // Packet length
		time_t get_header_timestamp() { return this->header->ts.tv_sec; }
		unsigned char *get_data() { return this->data; }
		unsigned char *get_packet() { return this->data; }
		bool get_filtered() { return this->filtered; }
		std::string get_protocol() { return this->protocol; }
		std::string get_info() { return this->info; }
		std::string get_fgcolour() { return this->fgcolour; } 
		std::string get_bgcolour() { return this->bgcolour; } 
		struct ether_header *get_ether_header() { return this->ether_header; }
		struct ip_header *get_ip_header() { 
			if (this->ip_header)
				return this->ip_header;
			return nullptr;
		}

		// Setters
		void set_filtered(bool flag) { this->filtered = flag; }
		void set_info(std::string information) { this->info = information; }
		void set_color(std::string col) { this->colour = col; }

		// Methods
		virtual void parse();

	private:

		// Properties

		int frame = -1;							// Position packet received
		struct pcap_pkthdr *header = nullptr; 	// Pointer to packet header struct
		unsigned char *data = nullptr;			// Pointer to packet data 

		struct ether_header *ether_header;
		struct ip_header *ip_header;

		std::string protocol = "";				// Protocol Type
		std::string info = "";					// Extra information filled at instantiation
		std::string fgcolour = "";				// Packet displayed colour Foreground (for GUI)
		std::string bgcolour = "";				// Packet displayed colour Background (for GUI)
		bool filtered = false;					// Filter flag

		unsigned char error_type;

};

class IPPacket : public Packet {
	public:

		IPPacket(int frame,
				struct pcap_pkthdr header,
				unsigned char *data, 
				unsigned char error_type = Error::CLI,
				std::string fgcolour = "",
				std::string bgcolour = "" 
				) : Packet(frame, header, data, error_type, "IP", fgcolour, bgcolour) {

		}

		~IPPacket() {} 

	private:
};

class ICMPPacket : public Packet {
	public:

		ICMPPacket(int frame,
				struct pcap_pkthdr header,
				unsigned char *data,
				unsigned char error_type = Error::CLI,
				std::string fgcolour = "",
				std::string bgcolour = ""
				) : Packet(frame, header, data, error_type, "ICMP", fgcolour, bgcolour) {

		}

		~ICMPPacket() {} 

	private:
};

class TCPPacket : public Packet {
	public:

		TCPPacket(int frame,
				struct pcap_pkthdr header,
				unsigned char *data,
				unsigned char error_type = Error::CLI,
				std::string prot = "TCP",
				std::string fgcolour = "",
				std::string bgcolour = ""
				) : Packet(frame, header, data, error_type, prot, fgcolour, bgcolour) {
			this->parse();
		}

		~TCPPacket() {} 

		// Getters 
		struct tcp_header *get_tcp_header() { return tcp_header; }

		// Methods 
		void parse() override;

	private:

		// Attributes
		struct tcp_header *tcp_header;

};

class HTTPPacket : public TCPPacket {
	public:
		HTTPPacket(int frame,
				struct pcap_pkthdr header,
				unsigned char *data,
				unsigned char error_type = Error::CLI,
				std::string prot = "HTTP",
				std::string fgcolour = "",
				std::string bgcolour = ""
				) : TCPPacket(frame, header, data, error_type, prot, fgcolour, bgcolour) {
			this->parse();
		}
		
		// Methods 
		void parse() override;

	private:

		// Attributes 
		unsigned char *http_payload;
};

class UDPPacket : public Packet {
	public:

		UDPPacket(int frame,
				struct pcap_pkthdr header,
				unsigned char *data,
				unsigned char error_type = Error::CLI, 
				std::string fgcolour = "",
				std::string bgcolour = ""
				) : Packet(frame, header, data, error_type, "UDP", fgcolour, bgcolour) {

		}

		~UDPPacket() {} 

	private:
};

class ARPPacket : public Packet {
	public:

		ARPPacket(int frame,
				struct pcap_pkthdr header,
				unsigned char *data,
				unsigned char error_type = Error::CLI,
				std::string fgcolour = "",
				std::string bgcolour = ""
				) : Packet(frame, header, data, error_type, "ARP", fgcolour, bgcolour) {

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

		void clear_packets() {
			this->packet_stream.clear();
		}

		unsigned int size() { return (unsigned int)this->packet_stream.size(); }

	private:
		// Methods 
		Packet *parse_tcp_packet(int frame, 
				struct pcap_pkthdr header, 
				unsigned char *data);

		// Properties 
		std::vector<Packet *> packet_stream;	// List of captured packet objects 
		unsigned char error_type;

};

#endif // PACKET_H
