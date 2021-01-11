#ifndef PROTOCOLS_H
#define PROTOCOLS_H

// https://unix.superglobalmegacorp.com/Net2/newsrc/netinet/if_ether.h.html
// Types in the Ether Header Structure
#define ETHERTYPE_ARP 0x0806
#define ETHERTYPE_IP  0x0800

struct ether_header {
	// Read in as LITTLE ENDIAN, but is sent as BIG ENDIAN. 
	// Keep this in mind!!!
	unsigned char dhost[6];	// Destination MAC Address
	unsigned char shost[6];	// Source MAC Address
	unsigned short type;	// Encapsulated Protocol Type
							// htons(type); to convert 
};

// http://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml
#define ICMP	1
#define IPv4	4
#define TCP		6
#define UDP		17

struct ip_header {
	// Read in as LITTLE ENDIAN, but is sent as BIG ENDIAN.
	// Keep this in mind!!!
	unsigned char type_helen;	// &0xF0 IP Type {First 4 bits}
								// &0x0F IP Header Length (32bit Words)
	
	unsigned char tos;	// Type Of Service
						// Also called Differentiated Service Code Point
						// or DSCP. Features for VoIP
	
	unsigned short len;	// Total length of packet. 20 < x < 65535
	unsigned short id;	// Used to identifiy packet fragments 

	unsigned short flag_frag_offset;	// &0b1110 0000 0000 0000 IP Flag {First 3 Bits}
										// &0b0001 1111 1111 1111 Fragment Offset
	
	unsigned char ttl;			// Time to live (seconds)
	unsigned char protocol;		// Protocol
	unsigned short checksum;	// Checksum for packet to check for errors
	unsigned int source;		// Source ip 
	unsigned int dest;			// Destination ip
};

#endif // PROTOCOLS_H
