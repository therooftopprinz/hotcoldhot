#include "App.hpp"

extern "C" void app_main()
{
    App app;
    app.run();
}

int main(int argc, char *argv[])
{
    app_main();
    return 0;
}
