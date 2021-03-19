#ifndef NETWORKING_H
#define NETWORKING_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <fstream>
#include <future>

#include <pcap.h>

#include <QObject>

#include "packet.hpp"
#include "Utils/utils.hpp"
#include "Utils/error.hpp"

class Networking : public QObject {
	Q_OBJECT; 

	public:
		Networking(unsigned char error_type = Error::CLI);
		~Networking();

		// Getters
		pcap_t *get_handle() { return this->handle; }
		pcap_if_t *get_device(int index) { 
			if (index >= 0 && index < (int) this->devices.size())
				return this->devices[index];
			return nullptr;
		}
		char *get_cur_device() { return this->selected_device; }
		char *get_cur_file() { return this->selected_file; }
		bpf_u_int32 get_subnet() { return this->subnet; }
		bpf_u_int32 get_netmask() { return this->netmask; }
		unsigned int get_packet_count() { return this->packet_count; }
		PacketStream *get_packet_stream() { return this->packet_stream; }
		bool get_select_file() { return this->select_file; }
		bool get_select_interface() { return this->select_interface; }

		// Setters
		bool set_subnet_netmask();
		void setup_device(int index, int timeout=1000, bool promiscuous=false); 
		void open_file(const char *filename);

		void clear_packets() { 
			this->packet_count = 0;
			this->get_packet_stream()->clear_packets();
		}

		// Methods
		void start_listening(int max_count=0);
		void start_listening(bool *active);
		int listen_next_packet();
		void save_packets_to_pcap(std::string filename);

		void create_new_dump();	// Creates a /tmp file to save dump to 
		void close_dump(std::string savefile); // Saves /tmp file to disk
		void close_dump_without_save(); // Close the dump file without saving
		
		int get_next_packet(unsigned char **packet, struct pcap_pkthdr *header);
		int set_filter(const char *expression, int optimize=0);

		// Miscellaneous
		void print_interfaces();

	private:
		// Methods
		int populate_interfaces();
		void open_live_device(int timeout, bool promiscuous);
		void open_offline_device(const char *filename);

		// Properties
		unsigned char error_type;			// Type of error for Error.hpp
		char errbuf[PCAP_ERRBUF_SIZE];		// Error buffer, holds last error

		pcap_t *handle = nullptr;			// pcap_t handle to the selected network interface
		std::vector<pcap_if_t *> devices;	// Current devices available on the computer

		bool select_file = false;
		bool select_interface = false; 
		int capture_packet_err_count = 0;	// Number of failed captures - used to detect EOF for .pcap files

		char selected_device[256] = "\0";	// String of the selected network interface
		char selected_file[256] = "\0";		// String of selected file to capture
		bpf_u_int32 subnet = 0;		// 32bit IP Subnet of selected network interface
		bpf_u_int32 netmask = 0;	// 32bit Network Mask of the selected network interface

		unsigned int packet_count = 0;	// Count of packets captured so far
		PacketStream *packet_stream;	// Stream / List of all captured packet objects
		pcap_dumper_t *dumper = nullptr;
		std::string tempfile;

		// Private structures
		struct pcap_hdr_s {
			unsigned int magic_number = 0xa1b2c3d4; /* magic number */
			unsigned short version_major = 0x2;		/* major version number */
			unsigned short version_minor = 0x4;		/* minor version number */
			int thiszone = 0x0;						/* GMT to local correction */
			unsigned int sigfigs = 0x0;				/* accuracy of timestamps */
			unsigned int snaplen = 0x00040000;		/* max bytes len of captured packets */
			unsigned int network = 0x01;			/* data link type */
		} pcap_hdr;


	signals:
		void packet_recv(Packet *packet); 
		void stopped_recv();

	public slots:

};

#endif // NETWORKING_H
