g++ -shared -o output/FileManager.so -fPIC src/FileManager/FileManager.cpp
echo "FileManager"
g++ -shared -o output/FileProcessor.so -fPIC src/FileProcessor/FileData.cpp src/FileProcessor/Reader.cpp
echo "FileProcessor"
g++ -shared -o output/CFBackend.so -fPIC src/CFBackend.cpp src/SectionData.cpp output/FileProcessor.so
echo "CFBackend"
g++ -shared -o output/CFEmbed.so -fPIC src/CFEmbed.cpp output/FileManager.so output/FileProcessor.so output/CFBackend.so
echo "CFEmbed"
g++ -c -o output/CloudyForest.o src/CF.cpp
g++ -o output/CloudyForest output/CloudyForest.o output/FileManager.so output/FileProcessor.so output/CFBackend.so
echo "CF"
