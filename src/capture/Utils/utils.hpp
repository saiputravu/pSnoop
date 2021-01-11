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

			for (unsigned int i = 0; i < length-8; i+=8) {
				for (int c = 0; c < 8; ++c)
					printf("%02x ", (unsigned char)string[i+c]);
				printf("|");

				for (int c = 0; c < 8; ++c) {
					if (0x20 <= (unsigned char)(string[i+c]) && (unsigned char)(string[i+c])<= 0x7e )
						printf("%c", (unsigned char)string[i+c]);
					else
						printf(".");
				}
				printf("|");

				printf("\n");
			}

			for (unsigned int i = length - (length%8); i < length; ++i)
				printf("%02x ", (unsigned char)string[i]);
			for (unsigned int i = length; i < length + (8 - (length%8)); i++) 
				printf("   ");
			printf("|");

			for (unsigned int i = length- (length%8); i < length; ++i) {
				if (0x20 <= (unsigned char)(string[i]) && (unsigned char)(string[i])<= 0x7e )
					printf("%c", (unsigned char)string[i]);
				else
					printf(".");
			}
			printf("|");
			printf("\n");
		}

		static std::string convert_bpf(bpf_u_int32 value) {
			struct in_addr ip_addr;
			ip_addr.s_addr = value;

			std::ostringstream stream;
			stream << inet_ntoa(ip_addr);
			return stream.str();
		}

		static std::string convert_ip(unsigned int ip) {
			std::string addr = "";
			addr += std::to_string((ip & 0xff000000) >> (6 * 4)) + '.';
			addr += std::to_string((ip & 0xff0000) >> (4 * 4)) + '.';
			addr += std::to_string((ip & 0xff00) >> (2 * 4)) + '.';
			addr += std::to_string(ip & 0xff);
			return addr;
		}
};

#endif // UTILS_H
