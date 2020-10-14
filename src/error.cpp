#include "error.hpp"

Error::Error() {

}

Error::~Error() {

}

void Error::handle_error(char *errbuf, int context) {
	switch (context) {
		case CLI:
			fprintf(stderr, "Error thrown: %s\n", errbuf);
			break;
		case GUI:
			break;
		default:
			break;
	}
}
