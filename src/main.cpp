#include "infrastructure/oscilloscope.hpp"

int main(int argc, char**argv){
    INFRA::Oscilloscope oscilloscope;
    return oscilloscope.run(argc, argv);
}
