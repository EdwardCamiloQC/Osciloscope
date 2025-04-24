#include <sys/oscilloscope.hpp>

int main(int argc, char**argv){
    Oscilloscope *myOscilloscope = Oscilloscope::getInstance();
    return myOscilloscope->run(argc, argv);
}
