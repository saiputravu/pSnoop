echo "Compiling ..."
mkdir -p ./bash
g++ src/*.cpp src/Utils/*.cpp -lpcap -o build/main
echo "Finished compiling!"
