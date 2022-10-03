#include <sequence.hpp>
#include <iostream>
#include <fstream>
#include <string>

struct Cell
{
    std::string text;
    int row, column;
};

std::string trim(const std::string & str)
{
    const char * WS = " \t";
    auto i = str.find_first_not_of(WS);
    if(i == std::string::npos) return "";
    auto j = str.find_last_not_of(WS);
    return str.substr(i, i-j+1);
}

void csvReader(const sequence<char> & input, const output_sequence<Cell> & output)
{
    int row=1;
    for(auto & line : input.split("\r\n"))
    {
        int column=1;
        for(auto &cell : seq(line).split(",").select(trim))
            output << Cell{cell,row,column++};
        row++;
    }
}

int main(int argc, const char**argv)
{
    int file=1;
    auto files = seq(argv, argc).skip(1);

    if(!files.any())
        std::cout << "Usage: csvreader <filename> ...\n";

    for(auto filename : files)
    {
        std::ifstream file(filename);
        csvReader(seq(file), receiver([](const Cell &cell) {
            std::cout << cell.row << "." << cell.column << ": " << cell.text << std::endl;
        }));
    }
}