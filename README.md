# pSnoop
A Level Project, PacketSnooper pSnoop

# Dependencies 

Please note, you may have to fix the /usr/include/boost/spirit.hpp and /usr/include/boost/spirit/ files.
```
sudo apt install qtchooser qt5-default libpcap-dev libboost-all-dev
```

# Compile
```
mkdir build
cd ./src
qmake && make
```

# Filter syntax and usage

## Compatible commands right now:
	* length | ==, >(=), <(=), != | Filters: packet length 
	* protocol | ==, != | Filters: packet protocol 
	* contains | ==, != | Filters: strings contained in packet
	* Wildcard: *
	* Chaining commands: &&

### Note: priority of chained commands are treated in chronological order

## Example Usage
```c
length == 102 && protocol == UDP
protocol == TCP
protocol == * // matches every packet - could be used to reset 
```
