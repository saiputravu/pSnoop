#include <iostream>
#include <cstdio>
#include <execinfo.h>

class Error {
	public:
		Error();
		~Error();

		static void handle_error(char *errbuf, int context, std::string prev="");

		enum context {
			CLI,
			GUI
		};

	private:

};


