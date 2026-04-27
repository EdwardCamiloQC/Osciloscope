#include <application/screen.hpp>
#include <adapters/gui.hpp>

using namespace SCREEN;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
Screen::Screen(std::unique_ptr<Gui> &&gui, [[maybe_unused]]unsigned int width, [[maybe_unused]]unsigned int heigth)
    : gui_(std::move(gui)), width_(width), heigth_(800)
{
    //
}

int Screen::turn_on(int &argc, char **&argv){
    return gui_->init(argc, argv);
}

int Screen::turn_off(){
    return 0;
}
