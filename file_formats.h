#pragma once

#include <string>

const std::string CMAKE_LISTS_TXT = R"(cmake_minimum_required(VERSION 3.26)
project(##PROJECT_NAME##)

set(CMAKE_CXX_STANDARD ##PROJECT_STD##)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(##PROJECT_NAME## src/main.cpp)
)";

const std::string CLANG_FORMAT = R"(BasedOnStyle: LLVM
AccessModifierOffset: -4
AlignAfterOpenBracket: AlwaysBreak
BinPackArguments: false
BinPackParameters: false
AlignConsecutiveAssignments: None
AlignOperands: Align
AllowAllArgumentsOnNextLine: true
AllowAllConstructorInitializersOnNextLine: false
AllowAllParametersOfDeclarationOnNextLine: true
AllowShortBlocksOnASingleLine: Empty
AllowShortCaseLabelsOnASingleLine: false
AllowShortFunctionsOnASingleLine: Empty
AllowShortIfStatementsOnASingleLine: Never
AllowShortLambdasOnASingleLine: All
AllowShortLoopsOnASingleLine: false
AlwaysBreakAfterReturnType: None
AlwaysBreakTemplateDeclarations: Yes
BreakBeforeBraces: Custom
BraceWrapping:
  AfterCaseLabel: false
  AfterClass: false
  AfterControlStatement: Never
  AfterEnum: false
  AfterFunction: false
  AfterNamespace: false
  AfterUnion: false
  BeforeCatch: false
  BeforeElse: false
  IndentBraces: false
  SplitEmptyFunction: false
  SplitEmptyRecord: false
BreakBeforeBinaryOperators: None
BreakBeforeTernaryOperators: true
BreakConstructorInitializers: BeforeColon
BreakInheritanceList: BeforeColon
ColumnLimit: 100
CompactNamespaces: false
ContinuationIndentWidth: 8
IndentCaseLabels: true
IndentPPDirectives: None
IndentWidth: 4
KeepEmptyLinesAtTheStartOfBlocks: false
MaxEmptyLinesToKeep: 1
NamespaceIndentation: All
ObjCSpaceAfterProperty: false
ObjCSpaceBeforeProtocolList: true
PointerAlignment: Left
ReflowComments: false
SpaceAfterCStyleCast: true
SpaceAfterLogicalNot: false
SpaceAfterTemplateKeyword: false
SpaceBeforeAssignmentOperators: true
SpaceBeforeCpp11BracedList: false
SpaceBeforeCtorInitializerColon: true
SpaceBeforeInheritanceColon: true
SpaceBeforeParens: ControlStatements
SpaceBeforeRangeBasedForLoopColon: true
SpaceInEmptyParentheses: false
SpacesBeforeTrailingComments: 2
SpacesInAngles: false
SpacesInCStyleCastParentheses: false
SpacesInContainerLiterals: false
SpacesInParentheses: false
SpacesInSquareBrackets: false
TabWidth: 4
UseTab: Never
)";

const std::string MAINCPP = R"(int main(int argc, char* argv[]) {
    return 0;
}
)";

const std::string LAUNCH_JSON = R""({
    "configurations": [
        {
            "type": "lldb",
            "request": "launch",
            "name": "Debug (##PROJECT_NAME##)",
            "program": "${workspaceFolder}/build/##PROJECT_NAME##.exe",
            "args": [],
            "cwd": "${workspaceFolder}"
        }
    ]
}
)"";

const std::string SETTINGS_JSON = R"({
  "editor.formatOnSave": true,
  "editor.defaultFormatter": "xaver.clang-format"
}
)";