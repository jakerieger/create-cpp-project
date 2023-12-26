#include "file_formats.h"
#include "result.hpp"
#include "utility.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <direct.h>
// Microsoft wants us to use _getcwd instead of getcwd, which breaks POSIX
// compatibility. See the following link for more information:
// https://stackoverflow.com/questions/7582394/strdup-or-strdup
// Therefore we must disable the compiler warning if we want to use getcwd
// to maintain POSIX compatibility. This is accomplished with the following
// line.
#pragma warning(disable : 4996)
#else
#include <unistd.h>
#endif

#define BUF_SIZE 260

using namespace cpp;
using args_t = std::vector<string>;

struct configuration {
    string project_name;
    CXX_STANDARD cxx_standard;
};

struct directories {
    string project_path;
    string vscode_path;
    string src_path;
};

result<configuration, error> process_args(args_t);
result<string, error> get_cwd();
result<directories, error> create_directories(const string&, const string&);
result<void, error> create_files(const directories&, const configuration&);

//==============================================================//
//                            MAIN                              //
//==============================================================//
int main(int argc, char* argv[]) {
    if (argc <= 2) {
        printf("error: No arguments or too few provided\n");
        return 1;
    }

    args_t arguments(argv + 1, argv + argc);
    auto process_args_result = process_args(arguments);
    if (process_args_result.has_error()) {
        printf("Failed to process arguments: '%s'\n",
               process_args_result.error().message().c_str());
        return 1;
    }

    auto config = process_args_result.value();

    // Create project directories
    auto get_cwd_result = get_cwd();
    if (get_cwd_result.has_error()) {
        printf("error: '%s'\n", get_cwd_result.error().message().c_str());
        return 1;
    }

    string cwd = get_cwd_result.value();

    auto create_dirs_result = create_directories(cwd, config.project_name);
    if (create_dirs_result.has_error()) {
        printf("error: '%s'\n", create_dirs_result.error().message().c_str());
        return 1;
    }

    directories dirs = create_dirs_result.value();

    // Create project files
    auto create_files_result = create_files(dirs, config);
    if (create_files_result.has_error()) {
        printf("error: '%s'\n", create_files_result.error().message().c_str());
        return 1;
    }

    // TODO: implement opening project dir in VSCode automatically

    return 0;
}

result<directories, error> create_directories(const string& cwd, const string& project_name) {
    fs::path project_path = join(cwd, project_name);
    fs::create_directory(project_path);

    // Create subdirectories
    fs::path vscode_path = join(project_path.string(), ".vscode");
    fs::path src_path = join(project_path.string(), "src");
    fs::path inc_path = join(project_path.string(), "include");
    fs::path vend_path = join(project_path.string(), "vendor");
    fs::create_directory(vscode_path);
    fs::create_directory(src_path);
    fs::create_directory(inc_path);
    fs::create_directory(vend_path);

    return directories{project_path.string(), vscode_path.string(), src_path.string()};
}

result<void, error> create_files(const directories& dirs, const configuration& config) {
    // Create .clang-format
    string clang_format_path = dirs.project_path + "\\.clang-format";
    std::ofstream clang_format_file(clang_format_path);
    clang_format_file << CLANG_FORMAT;
    clang_format_file.close();

    // Create CMakeLists.txt
    string cmake_lists_txt = CMAKE_LISTS_TXT;
    cmake_lists_txt = find_replace(cmake_lists_txt, "##PROJECT_NAME##", config.project_name);
    cmake_lists_txt = find_replace(
            cmake_lists_txt, "##PROJECT_STD##", std::to_string((uint8_t) config.cxx_standard));

    string cmake_lists_path = dirs.project_path + "\\CMakeLists.txt";
    std::ofstream cmake_lists_file(cmake_lists_path);
    cmake_lists_file << cmake_lists_txt;
    cmake_lists_file.close();

    // Create main source file
    string maincpp_path = dirs.src_path + "\\main.cpp";
    std::ofstream maincpp_file(maincpp_path);
    maincpp_file << MAINCPP;
    maincpp_file.close();

    // Create vscode config files
    string launch_json = LAUNCH_JSON;
    launch_json = find_replace(launch_json, "##PROJECT_NAME##", config.project_name);
    string launch_json_path = dirs.vscode_path + "\\launch.json";
    std::ofstream launch_json_file(launch_json_path);
    launch_json_file << launch_json;
    launch_json_file.close();

    string settings_json_path = dirs.vscode_path + "\\settings.json";
    std::ofstream settings_json_file(settings_json_path);
    settings_json_file << SETTINGS_JSON;
    settings_json_file.close();

    return {};
}

result<string, error> get_cwd() {
    char buffer[BUF_SIZE];
    char* p;
    p = getcwd(buffer, BUF_SIZE);
    if (p == nullptr) {
        return fail(error("Could not get current working directory"));
    }

    return string(buffer);
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