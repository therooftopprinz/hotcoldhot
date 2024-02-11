set(COMPONENT_ADD_INCLUDEDIRS ".")
set(COMPONENT_SRCS
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
)

set(COMPONENT_REQUIRES "lvgl")

register_component()
