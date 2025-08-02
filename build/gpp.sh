g++ -shared -o output/FileManager.so -fPIC src/FileManager/FileManager.cpp
g++ -shared -o output/CFBackend.so -fPIC src/CFBackend.cpp src/SectionData.cpp
g++ -shared -o output/CFEmbed.so -fPIC src/CFEmbed.cpp output/FileManager.so output/CFBackend.so
g++ -o output/CloudyForest src/CF.cpp output/FileManager.so output/CFBackend.so
