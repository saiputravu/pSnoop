echo "Compiling ..."
mkdir -p ./build
g++ src/*.cpp src/Utils/*.cpp -lpcap -pthread -std=c++0x -o build/main
echo "Finished compiling!"
