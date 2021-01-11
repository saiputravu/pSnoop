#include "error.hpp"

Error::Error() {

}

Error::~Error() {

}

void Error::handle_error(char *errbuf, int context, std::string prev) {
	switch (context) {
		case CLI:
			fprintf(stderr, "Error thrown in [%s]: %s\n", prev.c_str(), errbuf);
			break;
		case GUI:
			break;
		default:
			break;
	}
}
