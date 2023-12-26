# create-cpp-project

Simple command-line tool for quickly bootstrapping C++ projects in VS Code. Currently only setup for my desired preferences, check [TODO](#todo) for coming changes to make this usable by the community and not just me.

Requires Clang installed as a configurable compiler and the following extensions installed/configured in VSCode:

- [Clang-Format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format)
- [clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd)eez nuts
- [CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake)
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
- [CodeLLDB](https://marketplace.visualstudio.com/items?itemName=vadimcn.vscode-lldb)

## TODO

* [ ] Fix pathing (currently only creates project directory relative to `create-cpp-project` executable)
* [ ] Implement customizable file templates instead of hard-coded string literals
* [ ] Provide a more general/robust config setup (maybe read from an INI or something)
* [ ] Add a flag for launching VSCode automatically

## License
create-cpp-project is licensed under the DWTFYW (Do-Whatever-The-Fuck-You-Want) license.