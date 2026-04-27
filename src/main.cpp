#include <application/oscilloscope.hpp>

using namespace OSCILLOSCOPE;

int main(int argc, char**argv){
    Oscilloscope *oscilloscope = Oscilloscope::get_instance();
    return oscilloscope->run(argc, argv);
}
