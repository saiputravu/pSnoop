#include "networking.hpp"
#include "error.hpp"

Networking::Networking() {
	this->populate_interfaces();
}

Networking::~Networking() {
	pcap_freealldevs(this->devices[0]);
}

int Networking::populate_interfaces() {
	pcap_if_t *temp_dev, *temp;
	if (pcap_findalldevs(&temp_dev, this->errbuf) == -1) {
		Error::handle_error(this->errbuf, Error::CLI);	
		return -1;
	}

	for(temp = temp_dev; temp; temp = temp->next)
		(this->devices).push_back(temp);

	return 0;
}

void Networking::print_interfaces() {
	std::cout << "Available devices list: " << std::endl;
	for (int i = 0; i < (this->devices).size(); ++i)
		std::cout << "\t[" << i << "] " << (this->devices)[i]->name << std::endl;
}
