#ifndef __APP_HPP__
#define __APP_HPP__

#include "ui/UI.hpp"

class App
{
public:
    App();
    ~App();
    void run();
private:
    ui::UI ui;
};

#endif // __APP_HPP__