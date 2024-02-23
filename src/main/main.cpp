#include <osciloscope.h>

int main(){
    WindowOsciloscope osciloscope(1000, 800, "/dev/ttyACM0");
    osciloscope.run();
    return EXIT_SUCCESS;
}
