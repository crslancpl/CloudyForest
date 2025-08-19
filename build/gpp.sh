cd cf
g++ -fPIC -c ../src/FileManager/FileManager.cpp
echo "FileManager"
g++ -fPIC \
 -c ../src/FileProcessor/FileData.cpp ../src/FileProcessor/Tokenizer.cpp ../src/FileProcessor/Codes.cpp ../src/FileProcessor/Interpret.cpp ../src/Tools.cpp
echo "FileProcessor"
g++ -fPIC -c ../src/CFBackend.cpp ../src/SectionData.cpp
echo "CFBackend"

g++ -fPIC -c ../src/CFEmbed.cpp

g++ -shared -fPIC -o libCFEmbed.so CFEmbed.o FileManager.o FileData.o Tokenizer.o Codes.o Interpret.o Tools.o CFBackend.o SectionData.o
echo "CFEmbed"

g++ -o CloudyForest ../src/CF.cpp FileManager.o FileData.o Tokenizer.o Codes.o Interpret.o Tools.o CFBackend.o SectionData.o CFEmbed.o
echo "CF"
