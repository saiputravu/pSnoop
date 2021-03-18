#ifndef PROTOCOLS_H
#define PROTOCOLS_H

/*
 * Key points to note about network protocol parsing:
 *
 * 1. Network packets are BIG ENDIAN
 * 2. Localhost packets are LITTLE ENDIAN
 * 3. C reads into structures as LITTLE ENDIAN
 *
 */

// https://unix.superglobalmegacorp.com/Net2/newsrc/netinet/if_ether.h.html
// Types in the Ether Header Structure
#define ETHERTYPE_ARP 0x0806
#define ETHERTYPE_IP  0x0800

struct ether_header {
	unsigned char dhost[6];	// Destination MAC Address
	unsigned char shost[6];	// Source MAC Address
	unsigned short type;	// Encapsulated Protocol Type
							// htons(type); to convert 
};

// http://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml
#define ICMP	1
#define TCP		6
#define UDP		17

struct ip_header {
	unsigned char type_helen;	// &0xF0 IP Type {First 4 bits}
								// &0x0F IP Header Length (32bit Words)
	
	unsigned char tos;	// Type Of Service
						// Also called Differentiated Service Code Point
						// or DSCP. Features for VoIP
	
	unsigned short len;	// Total length of packet. 20 < x < 65535
	unsigned short id;	// Used to identifiy packet fragments 

	unsigned short flag_frag_offset;	// &0b111- ---- ---- ---- IP Flag {First 3 Bits}
										// &0b---1 1111 1111 1111 Fragment Offset
	
	unsigned char ttl;			// Time to live (seconds)
	unsigned char protocol;		// Protocol
	unsigned short checksum;	// Checksum for packet to check for errors
	unsigned int source;		// Source ip 
	unsigned int dest;			// Destination ip

	// This is only valid when helen > 5 | Do NOT use sizeof() for this struct size, use helen*4
	unsigned int options_padding;	// Padding is all 00s, size depends on options
};

// https://tools.ietf.org/html/rfc793#section-3.1
struct tcp_header {
	unsigned short source_port; // Source port
	unsigned short dest_port; // Destination port 

	unsigned int seq;	// Sequence number of packet in the stream. 
						// If SYN flag set, sequence number is ISN (Initial sequence number).

	unsigned int ack;	// Acknowledge number 
						// If ACK flag set, this contains value of next sequence number. Always sent once a connection is established

	unsigned short offset_resv_control; // 0b1111 ---- ---- ---- Data offset : Number of 32bit words in TCP header (TCP header len = 4*data offset)
										// 0b---- 1111 11-- ---- Reserved
										// 0b---- ---- --11 1111 TCP Flags | URG,ACK,PSH,RST,SYN,FIN
	
	unsigned short window; // The number of data octets beginning with the one indicated in the ack field which sender is willing to ACCEPT
	unsigned short checksum; // Checksum - 16bit 1's complement of the 1's complement sum of all 16 bit words in header and text

	unsigned short urgent_pointer;

	// This is only valid some times
	unsigned int options_padding; // Padding is all 00s, size depends on options
};

#endif // PROTOCOLS_H
