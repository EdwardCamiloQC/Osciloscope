#include <oscilloscope.hpp>

int main(int argc, char**argv){
    Oscilloscope *myOscilloscope = Oscilloscope::get_instance();
    return myOscilloscope->run(argc, argv);
}
