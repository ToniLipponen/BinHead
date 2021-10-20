/*
    Little program to turn raw file data to a static array in a C/C++ header file.
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

void PrintUsage()
{
    std::cout <<
    R"END(
    Usage:  BinHead $file_name$ or
            BinHead $file_name$ $output_name$

            This outputs a $output_name$.h file, which you can include in your project.
    )END"
    << std::endl;
}

int main(int argc, char** argv)
{
    if(argc < 2)
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

    std::string filename, filenameUpper;
    std::ofstream output;
    if(argc == 2)
        filename = "data_header";
    else if(argc == 3)
        filename = std::string(argv[2]);

    output.open(filename + ".h");
    filenameUpper = filename;

    std::transform(filename.begin(), filename.end(), filenameUpper.begin(), ::toupper);

    output << "#ifndef " + filenameUpper + "_H\n#define " + filenameUpper + "_H \n\nstatic const unsigned char " + filename + "_DATA[] = {\n\t";
    for(int i = 0; i < file_len; i++)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << int(buffer[i]) << ", ";
        if(i % 15 == 0 && i != 0)
            ss << "\n\t";
        output << ss.str();
    }
    output << "\n};\nunsigned int " + filename + "_LENGTH = " + std::to_string(file_len) + ";\n#endif // " + filename + "_H";
    output.close();
    delete[] buffer;
    return 0;
}