echo "Compiling ..."
mkdir -p ./build
g++ src/*.cpp src/Utils/*.cpp -lpcap -o build/main
echo "Finished compiling!"
