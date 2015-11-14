
//
// Disclamer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//

#include "GameEngine.hpp"

int main(int, char const**)
{
    ContextSettings settings;
    
    settings.antialiasingLevel = 5;
    
    GameEngine  engine(settings);
    
    if (!engine.init())
        return (EXIT_FAILURE);
    engine.start();


    return (EXIT_SUCCESS);
}
