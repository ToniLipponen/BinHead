/*
    Little program to turn raw file data to a static array in a C/C++ header file.
*/

#include <iostream>
#include <sstream>
#include <fstream>

void PrintUsage()
{
    std::cout << "\n\nUsage: ./BinHeader $filename$\nThe program outputs a 'data_header.h' file if successful, and outputs an error message if the file could not be opened.\n\n\n";
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        PrintUsage();
        return 0;
    }
    unsigned char* buffer;
    std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
    if(!file.is_open())
    {
        std::cout << "Error: Could not open file -> " << argv[1] << std::endl;
        file.close();
        return 1;
    }
    const auto file_len = file.tellg();
    file.seekg(0, std::ios::beg);
    buffer = new unsigned char[file_len];
    file.read(reinterpret_cast<char*>(buffer), file_len);
    file.close();

    std::ofstream output("data_header.h");
    output << "#ifndef BINARY_HEADER_DATA_H\n#define BINARY_HEADER_DATA_H\n\nstatic const unsigned char my_data[] = {\n\t";
    for(int i = 0; i < file_len; i++)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << int(buffer[i]) << ", ";
        if(i % 15 == 0 && i != 0)
            ss << "\n\t";
        output << ss.str();
    }
    output << "\n};\n\n#endif";
    output.close();
    return 0;
}