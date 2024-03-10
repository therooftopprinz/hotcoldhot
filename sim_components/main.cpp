#include "App.hpp"

extern "C" void setup();
extern "C" void loop();

int main(int argc, char *argv[])
{
    setup();
    while(true) loop();
    return 0;
}
