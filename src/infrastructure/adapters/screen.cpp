#include "infrastructure/adapters/screen.hpp"
#include "application/IGui.hpp"

using namespace INFRA;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
Screen::Screen(std::unique_ptr<APP::IGui> &&gui, [[maybe_unused]]unsigned int width, [[maybe_unused]]unsigned int heigth)
    : guiPtr_(std::move(gui)),
    width_(width),
    heigth_(800)
{
    //
}

void Screen::select_gui(std::unique_ptr<APP::IGui> &&gui){
    guiPtr_ = std::move(gui);
}

void Screen::associate_signal_capturer(APP::ISignalCapturer* signalCapturerPtr){
    if(guiPtr_.get() != nullptr){
        guiPtr_->associate_signal_capturer(signalCapturerPtr);
    }
}

void Screen::associate_dev_inspector(APP::IDevInspector* devInspPtr){
    if(guiPtr_.get() != nullptr)
        guiPtr_->associate_dev_inspector(devInspPtr);
}

void Screen::associate_doc_generator(APP::IDocGenerator* docGenPtr){
    if(guiPtr_.get() != nullptr)
        guiPtr_->associate_doc_generator(docGenPtr);
}

void Screen::associate_voltages(DOMN::VoltageSignal* voltsPtr){
    if(guiPtr_.get() != nullptr)
        guiPtr_->associate_voltages(voltsPtr);
}

int Screen::turn_on(int &argc, char **&argv){
    return guiPtr_->init(argc, argv);
}

int Screen::turn_off(){
    return 0;
}

void Screen::add_device(const char* devName){
    if(guiPtr_.get() != nullptr){
        guiPtr_->update_drop_port(true, devName);
    }
}

void Screen::remove_device(const char* devName){
    if(guiPtr_.get() != nullptr){
        guiPtr_->update_drop_port(false, devName);
    }
}

void Screen::update_port_state(bool state){
    if(guiPtr_)
        guiPtr_->update_label_button_port(state);
}

long Screen::get_period_time_ns(){
    if(guiPtr_ == nullptr)
        return 10000000L; //10ms

    return guiPtr_->get_period_time_cap_ns();
}

void Screen::set_message(const char* msg, int type) const{
    if(guiPtr_ != nullptr)
        guiPtr_->display_message(msg, type);
}
