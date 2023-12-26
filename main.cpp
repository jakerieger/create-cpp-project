#include "file_formats.h"
#include "result.hpp"
#include "utility.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

using namespace cpp;
using args_t = std::vector<string>;

struct configuration {
    string project_name;
    CXX_STANDARD cxx_standard;
};

result<configuration, error> process_args(args_t);
result<string, error> exec_command(const char*);

//==============================================================//
//                            MAIN                              //
//==============================================================//
int main(int argc, char* argv[]) {
    if (argc <= 2) {
        printf("error: No arguments or too few provided\n");
        return 1;
    }

    namespace fs = std::filesystem;

    args_t arguments(argv + 1, argv + argc);
    auto process_args_result = process_args(arguments);
    if (process_args_result.has_error()) {
        printf("Failed to process arguments: '%s'\n",
               process_args_result.error().message().c_str());
        return 1;
    }

    auto config = process_args_result.value();

    // Create project directory
    fs::path project_path = fs::current_path().append(config.project_name);
    fs::create_directory(project_path);

    // Create subdirectories
    fs::path vscode_path = fs::current_path().append(config.project_name).append(".vscode");
    fs::path src_path = fs::current_path().append(config.project_name).append("src");
    fs::path inc_path = fs::current_path().append(config.project_name).append("include");
    fs::path vend_path = fs::current_path().append(config.project_name).append("vendor");
    fs::create_directory(vscode_path);
    fs::create_directory(src_path);
    fs::create_directory(inc_path);
    fs::create_directory(vend_path);

    // Create .clang-format
    string clang_format_path = project_path.string() + "\\.clang-format";
    std::ofstream clang_format_file(clang_format_path);
    clang_format_file << CLANG_FORMAT;
    clang_format_file.close();

    // Create CMakeLists.txt
    string cmake_lists_txt = CMAKE_LISTS_TXT;
    cmake_lists_txt = find_replace(cmake_lists_txt, "##PROJECT_NAME##", config.project_name);
    cmake_lists_txt = find_replace(
            cmake_lists_txt, "##PROJECT_STD##", std::to_string((uint8_t) config.cxx_standard));

    string cmake_lists_path = project_path.string() + "\\CMakeLists.txt";
    std::ofstream cmake_lists_file(cmake_lists_path);
    cmake_lists_file << cmake_lists_txt;
    cmake_lists_file.close();

    // Create main source file
    string maincpp_path = src_path.string() + "\\main.cpp";
    std::ofstream maincpp_file(maincpp_path);
    maincpp_file << MAINCPP;
    maincpp_file.close();

    // Create vscode config files
    string launch_json = LAUNCH_JSON;
    launch_json = find_replace(launch_json, "##PROJECT_NAME##", config.project_name);
    string launch_json_path = vscode_path.string() + "\\launch.json";
    std::ofstream launch_json_file(launch_json_path);
    launch_json_file << launch_json;
    launch_json_file.close();

    string settings_json_path = vscode_path.string() + "\\settings.json";
    std::ofstream settings_json_file(settings_json_path);
    settings_json_file << SETTINGS_JSON;
    settings_json_file.close();

    // TODO: implement opening project dir in VSCode automatically

    return 0;
}

result<configuration, error> process_args(args_t args) {
    configuration config{};

    for (uint8_t i = 0; i < args.size(); i++) {
        auto arg = args[i];

        if (arg == "--name") {
            if (args.size() <= i + 1) {
                return fail(error("Flag provided but no value was given"));
            }

            config.project_name = args[++i];
        }

        if (arg == "--std") {
            if (args.size() <= i + 1) {
                return fail(error("Flag provided but no value was given"));
            }

            uint8_t as_int = atoi(args[++i].c_str());
            if (as_int == 0) {
                return fail(error("Could not parse provided --std value"));
            }
            config.cxx_standard = (CXX_STANDARD) as_int;
        }
    }

    return config;
}

result<string, error> exec_command(const char* cmd) {
    using namespace std;
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        return fail(error("popen() failed"));
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}