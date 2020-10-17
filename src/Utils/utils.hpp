#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>

#include <pcap.h>
#include <arpa/inet.h>

class Utils {
public:
	static char *convert_time(time_t seconds) { return ctime(&seconds); }

	static void hexdump(unsigned char *string, unsigned int length) {
		
		std::string out((char *)string, length);
		for (int i = 0; i < (length % 8); ++i)
			out.push_back(0x00);

		for (int i = 0; i < length; i+=8) {
			for (int c = 0; c < 8; ++c)
				printf("%02x ", (unsigned char)out[i+c]);
			printf("| ");

			for (int c = 0; c < 8; ++c) {
				if (isalnum((unsigned char)out[i+c]))
					printf("%c ", (unsigned char)out[i+c]);
				else
					printf(". ");
			}
			printf("|");

			printf("\n");
		}
	}

	static std::string convert_bpf(bpf_u_int32 value) {
		struct in_addr ip_addr;
		ip_addr.s_addr = value;
		
		std::ostringstream stream;
		stream << inet_ntoa(ip_addr);
		return stream.str();
	}

};

#endif
