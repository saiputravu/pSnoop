#include "error.hpp"

Error::Error() {

}

Error::~Error() {

}

void Error::handle_error(char *errbuf, int context, const char *prev) {
	switch (context) {
		case CLI:
			fprintf(stderr, "Error thrown in [%s]: %s\n", prev, errbuf);
			break;
		case GUI:
			break;
		default:
			break;
	}
}
