#include <fcntl.h>  //Control de archivos (open,, O_RDONLY)
#include <unistd.h> //read, close
#include <iomanip>  //Para formatear la salida hex
#include <iostream>

#include <infrastructure/serialPort/serialPortPsoc.hpp>
#include <application/oscilloscope.hpp>

using namespace SERIAL_PORT;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
SerialPortPsoc::SerialPortPsoc(){
    state_ = false;
}

SerialPortPsoc::~SerialPortPsoc(){
    if(fd_ > 0){
        close_port();
        state_ = false;
    }
    std::cout << "Destructor SerialPsoc\n";
}

int SerialPortPsoc::open_port(const char* portName){
    fd_ = open(portName, O_RDONLY | O_NOCTTY /*| O_NONBLOCK*/);

    if(fd_ == -1){
        std::cerr << "Error: No se pudo abrir el puerto.\n";
        return EXIT_FAILURE;
    }

    if(tcgetattr(fd_, &tty_) != 0){
        std::cerr << "Error: tcgetattr.\n";
        return EXIT_FAILURE;
    }

    tcflush(fd_, TCIOFLUSH);

    cfmakeraw(&tty_);

    tty_.c_cc[VMIN] = 8; //bloquea hasta recibir exactamente 8bytes
    tty_.c_cc[VTIME] = 1; //timeout de 0.1 segundos entre bytes

    if(tcsetattr(fd_, TCSANOW, &tty_) != 0){
        std::cerr << "Error: tcsetattr.\n";
        return EXIT_FAILURE;
    }

    state_ = true;

    return EXIT_SUCCESS;
}

int SerialPortPsoc::close_port(){
    tcflush(fd_, TCIOFLUSH);
    state_ = false;
    return close(fd_);
}

void SerialPortPsoc::catch_data(void* userData){
    auto oscPt = reinterpret_cast<OSCILLOSCOPE::Oscilloscope*>(userData);

    uint8_t buffer[8] = {0};

    try{
        ssize_t m = read(fd_, buffer, sizeof(buffer));
        if(m == sizeof(buffer)){
            /*for(int i = 0; i < m; i++){
                std::cout<< std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i]) << " ";
            }
            std::cout << std::dec << std::endl;*/
            float data1 = (static_cast<float>(static_cast<uint16_t>(buffer[6] | buffer[7]<<8)) * 5.0f / 2047.0f) - 5.0f;
            float data2 = (static_cast<float>(static_cast<uint16_t>(buffer[4] | buffer[5]<<8)) * 5.0f / 2047.0f) - 5.0f;
            float data3 = (static_cast<float>(static_cast<uint16_t>(buffer[2] | buffer[3]<<8)) * 5.0f / 2047.0f) - 5.0f;
            float data4 = (static_cast<float>(static_cast<uint16_t>(buffer[0] | buffer[1]<<8)) * 5.0f / 2047.0f) - 5.0f;
            {
                std::lock_guard<std::mutex> lock(oscPt->mutex_);
                oscPt->voltage1_.ringBuffer_.push_data(&data1, 1);
                oscPt->voltage2_.ringBuffer_.push_data(&data2, 1);
                oscPt->voltage3_.ringBuffer_.push_data(&data3, 1);
                oscPt->voltage4_.ringBuffer_.push_data(&data4, 1);
            }
        }else{
            float cero = 0.0f;
            {
                std::lock_guard<std::mutex> lock(oscPt->mutex_);
                oscPt->voltage1_.ringBuffer_.push_data(&cero, 1);
                oscPt->voltage2_.ringBuffer_.push_data(&cero, 1);
                oscPt->voltage3_.ringBuffer_.push_data(&cero, 1);
                oscPt->voltage4_.ringBuffer_.push_data(&cero, 1);
            }
        }
    }catch(...){
        std::cerr << "Error en lectura de datos.\n";
    }
}

void SerialPortPsoc::set_data([[maybe_unused]]void* userData){
    //
}

bool SerialPortPsoc::get_flag_serial(){
    return state_;
}

IdCapturer_t SerialPortPsoc::get_Id(){
    return IdCapturer_t::SERIAL_PORT2_ID;
}
