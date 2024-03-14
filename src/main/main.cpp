#include <oscilloscope.h>

int main(int argc, char**argv){
    int status=0;
    Oscilloscope miOsc(argc, argv, "/dev/ttyACM0");
    status = miOsc.oscilloscope_run(argc, argv);
    return status;
}
