#include <iostream>
#include <cstdio>

class Error {
public:
	Error();
	~Error();
	
	static void handle_error(char *errbuf, int context);


	enum context {
		CLI,
		GUI
	};
	
private:

};


