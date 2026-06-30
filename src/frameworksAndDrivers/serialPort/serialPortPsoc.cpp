#include <fcntl.h>  //Control de archivos (open,, O_RDONLY)
#include <unistd.h> //read, close
#include <iomanip>  //Para formatear la salida hex
#include <assert.h>
#include <iostream>
#include "frameworksAndDrivers/serialPort/serialPortPsoc.hpp"
#include "infrastructure/adapters/screen.hpp"
#include "infrastructure/adapters/signalCapturer.hpp"
#include "domain/signals/voltageSignal.hpp"

using namespace DRV_FRAMW;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
SerialPortPsoc& SerialPortPsoc::get_instance(){
    static SerialPortPsoc instance;
    return instance;
}

APP::MsgReturn_t SerialPortPsoc::open_port(const char* portName){
    if(!(portName))
        return APP::MsgReturn_t::DONT_NAME_PORT;

    fd_ = open(portName, O_RDONLY | O_NOCTTY /*| O_NONBLOCK*/);

    if(fd_ == -1){
        perror("open");
        return APP::MsgReturn_t::ERROR_IN_OPEN;
    }

    if(tcgetattr(fd_, &tty_) != 0){
        perror("tcgetattr");
        close(fd_);
        return APP::MsgReturn_t::ERROR_IN_OPEN;
    }

    cfmakeraw(&tty_);

    tty_.c_cflag &= ~CSIZE;
    tty_.c_cflag |= CS8;//tamaño de palabra de uart 8bits
    tty_.c_cflag &= ~PARENB;//paridad
    tty_.c_cflag &= ~CSTOPB;//bits de parada->2bits
    tty_.c_cflag &= ~CRTSCTS;
    tty_.c_cflag |= CREAD | CLOCAL;//CREAD hibilita receptor. CLOCAL ignora lineas de modem(DCD,DSR,CTS)

    tty_.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);//ICANON modo canonico. ECHO redirige a terminal. ISIG señales especiales

    tty_.c_iflag &= ~(IXON | IXOFF | IXANY);//IXON:habilita o deshabilita control de flujo por software.

    tty_.c_oflag &= ~OPOST;//Procesamiento de salida.

    tty_.c_cc[VMIN] = 0; //bloquea hasta recibir n bytes o el tiempo se acabe
    tty_.c_cc[VTIME] = 1; //timeout de 0.1 segundos a partir del primer byte.

    if(tcsetattr(fd_, TCSANOW, &tty_) != 0){
        perror("tcsetattr");
        close(fd_);
        return APP::MsgReturn_t::ERROR_IN_OPEN;
    }

    if(fd_ > 0){
        tcflush(fd_, TCIOFLUSH);
        return APP::MsgReturn_t::PORT_OPENED;
    }

    return APP::MsgReturn_t::ERROR_IN_OPEN;
}

APP::MsgReturn_t SerialPortPsoc::close_port(){
    tcflush(fd_, TCIOFLUSH);

    int val = close(fd_);

    if(val == 0){
        fd_ = -1;
    }

    if(get_flag_serial()){
        return APP::MsgReturn_t::ERROR_IN_CLOSE;
    }

    return APP::MsgReturn_t::PORT_CLOSED;
}

APP::MsgReturn_t SerialPortPsoc::catch_data(void* userData){
    auto dataPt = reinterpret_cast<INFRA::SignalCapturer*>(userData);
    assert(dataPt != nullptr);
    assert(dataPt->get_voltages_ref() != nullptr);

    static uint8_t buffer[64] = {0};
    static ssize_t dReceived = 0;
    float cero = 1.0f;

    if(get_flag_serial()){
        ssize_t m = read(fd_, buffer + dReceived, 64 - dReceived);
        if(m < 0){
            perror("read");
            return APP::MsgReturn_t::ERROR_IN_CATCH;
        }else if(m > 0){
            dReceived += m;
            if(dReceived == sizeof(buffer)){
                uint8_t numSignal = (buffer[1]>>5)-1;
                if((numSignal<4) && ((buffer[1]&0x11)==0x11)){
                    {
                        std::lock_guard<std::mutex> lock(dataPt->get_mutex());
                        dataPt->get_voltages_ref()[numSignal].ringBuffer_.push_data(&buffer[2], 31, 12, 5.0f);
                    }
                }
                dReceived = 0;
            }
        }
    }else{
        cero = 0.0;
        {
            std::lock_guard<std::mutex> lock(dataPt->get_mutex());
            dataPt->get_voltages_ref()[0].ringBuffer_.push_data(&cero, 1);
            dataPt->get_voltages_ref()[1].ringBuffer_.push_data(&cero, 1);
            dataPt->get_voltages_ref()[2].ringBuffer_.push_data(&cero, 1);
            dataPt->get_voltages_ref()[3].ringBuffer_.push_data(&cero, 1);
        }
    }

    return APP::MsgReturn_t::CATCH;
}

APP::MsgReturn_t SerialPortPsoc::set_data([[maybe_unused]]void* userData){
    if(userData == nullptr){
        return APP::MsgReturn_t::ERROR_IN_SEND;
    }

    return APP::MsgReturn_t::SEND;
}

bool SerialPortPsoc::get_flag_serial(){
    if(fd_ >= 0)
        return true;

    return false;
}

APP::IdCapturer_t SerialPortPsoc::get_Id(){
    return APP::IdCapturer_t::SERIAL_PORT_PSOC_ID;
}
//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
SerialPortPsoc::SerialPortPsoc(){
    fd_ = -1;
}

SerialPortPsoc::~SerialPortPsoc(){
    if(fd_ > 0){
        close_port();
    }
}
