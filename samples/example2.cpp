// This example demonstrates processing command line arguments as a sequence of strings.

#include <sequence.hpp>
#include <iostream>

// A function that processes a sequence of options and files
void processFiles(const sequence<std::pair<char, std::string>> & options, const sequence<std::string> & files)
{
    // Extract all options of the form "-Ixxx" into a sequence
    auto includes = options.
        where ([](const std::pair<char, std::string> &option) { return option.first=='I'; }).
        select([](const std::pair<char, std::string> &option) { return option.second; });

    // Display all includes
    for(auto & include : includes)
        std::cout << "Include " << include << std::endl;

    // Display all files
    for(auto & file : files)
        std::cout << "Open file " << file << std::endl;
}

int main(int argc, const char**argv)
{
    // Define a sequence that convert the arguments to a list of std::string,
    // skipping argv[0] which is the program itself
    auto arguments = seq(argv, argc).skip(1).as<std::string>();

    // Define the sequence of arguments before "--"
    auto beforeDashes = arguments.take_while([](const std::string & arg) { return arg != "--"; });

    // Define the sequence of arguments after "--"
    auto afterDashes = arguments.skip_until([](const std::string & arg) { return arg == "--"; }).skip(1);

    // Define sequence of options as anything before "--" that begins with a "-"
    // Convert each option into a pair of option name and value.
    auto options = beforeDashes.
        where ([](const std::string &arg) { return !arg.empty() && arg[0]=='-'; }).
        select([](const std::string &arg) { return std::make_pair(arg[1], arg.substr(2)); });

    // Define sequence of files as all non-option arguments + anything after "--"
    auto files = beforeDashes.where([](const std::string &arg) { return arg[0]!='-'; }) + afterDashes;

    processFiles(options, files);

    return 0;
}
