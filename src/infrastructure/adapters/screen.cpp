#include "infrastructure/adapters/screen.hpp"
#include "application/IGui.hpp"
#include "frameworksAndDrivers/gui/guiGTK.hpp"

using namespace INFRA;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
Screen::Screen(unsigned int width, unsigned int heigth)
    : gui_(DRV_FRAMW::GuiGtk::get_instance()),
    width_(width),
    heigth_(heigth)
{
    //
}

void Screen::associate_signal_capturer(APP::ISignalCapturer* signalCapturerPtr){
    gui_.associate_signal_capturer(signalCapturerPtr);
}

void Screen::associate_dev_inspector(APP::IDevInspector* devInspPtr){
    gui_.associate_dev_inspector(devInspPtr);
}

void Screen::associate_doc_generator(APP::IDocGenerator* docGenPtr){
    gui_.associate_doc_generator(docGenPtr);
}

void Screen::associate_voltages(DOMN::VoltageSignal* voltsPtr){
    gui_.associate_voltages(voltsPtr);
}

int Screen::turn_on(int &argc, char **&argv){
    return gui_.init(argc, argv);
}

int Screen::turn_off(){
    return 0;
}

void Screen::add_device(const char* devName){
    gui_.update_drop_port(true, devName);
}

void Screen::remove_device(const char* devName){
    gui_.update_drop_port(false, devName);
}

void Screen::update_port_state(bool state){
    gui_.update_label_button_port(state);
}

void Screen::set_message(const char* msg, int type) const{
    gui_.display_message(msg, type);
}
