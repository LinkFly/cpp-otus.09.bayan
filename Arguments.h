#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <boost/program_options.hpp>

namespace po = boost::program_options;
using std::cout;
using std::endl;
using std::function;
using std::string;
using std::vector;

string helpHeader = string{}
+ "Welcome to `bayan` program! It look for equal files\n"
+ "Usage:"
+ "./bayan --block-size <block_size> hash-type <crc32|md5|sha1> [--dir <directory>] | [<file>*]\n"
+ "General options";

struct Arguments {
    static inline po::options_description desc = po::options_description(helpHeader.c_str());
    static inline string hashType;
    static inline size_t blockSize;
    static inline vector<string> files;
    static inline string dir;

	static void parse(int argc, char** argv) {
        desc.add_options()
            ("help,h", "Show help")
            ("block-size,s", po::value<size_t>(&blockSize)->required(), "Select block size (must be > 0)")
            ("hash-type,t", po::value<std::string>(&hashType)->required(), "Select hashType: crc32, md5, sha1")
            ("dir,d", po::value<string>(&dir), "Directory with file for to recursive compare (without --file options)")
            ("file,f", po::value<vector<string>>(&files), "Files for compare (require multiple options, without --dir option)")
            ;

        // For unlimited files after --block-size, --hash-type (without `--file` before file)
        po::positional_options_description posOptions;
        posOptions.add("file", -1);

        po::variables_map vm;
        try {
            auto options = po::command_line_parser(argc, argv).options(desc).positional(posOptions);
            po::parsed_options parsed = options.run();
            po::store(parsed, vm);
            if (vm.count("help")) {
                showDesc();
                exit(0);
            }
            po::notify(vm);
            check_arguments(vm);
        }
        catch (std::exception & ex) {
            std::cerr << "ERROR: " << ex.what() << "\n\n";
            showDesc();
            exit(-1);
        }

	}

    static void showDesc() {
        std::cout << desc << std::endl;
    }

	static void check_arguments(const po::variables_map& vm) throw(std::exception)
	{
        if (blockSize == 0) {
            throw std::exception("Failed: --block-size must be > 0");
        }
        if (vm.count("dir") && vm.count("file")) {
            throw std::exception("Failed: denied both --dir and --file options");
        }
        if (vm.count("file") && vm["file"].as<vector<string>>().size() <= 1) {
            throw std::exception("Failed: --file options count must be > 1 (or --dir options must be have)");
        }
	}
};
