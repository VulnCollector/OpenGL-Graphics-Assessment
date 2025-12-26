// OpenGLApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Application.h"



int main() 
{
    Application* theApp = new Application();
    if (theApp->startup() == true) {
        while (theApp->update() == true)
            theApp->draw();
        theApp->shutdown();
    }

    delete theApp;
    return 0;
    
}