cmake_minimum_required(VERSION 3.16)

set(MAIN_INCLUDE ".")
set(MAIN_SRCS
    "app_main.cpp"
    "App.cpp"
    "ui/UI.cpp"
    "ui/gl/Button.cpp"
    "ui/gl/Keyboard.cpp"
    "ui/gl/Label.cpp"
    "ui/gl/MessageBox.cpp"
    "ui/gl/TabView.cpp"
    "ui/gl/TextArea.cpp"
    "ui/GlobalStyles.cpp"
    "ui/MenuProgram/MenuProgram.cpp"
    "ui/MenuProgram/ProgramBuilder.cpp")
idf_component_register(
        SRCS ${COMPONENT_SRCS}
        INCLUDE_DIRS ${MAIN_INCLUDE})