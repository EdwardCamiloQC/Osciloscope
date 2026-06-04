#include <fcntl.h>  //Control de archivos (open,, O_RDONLY)
#include <unistd.h> //read, close
#include <iomanip>  //Para formatear la salida hex
#include <assert.h>
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
        return APP::MsgReturn_t::ERROR_IN_OPEN;
    }

    if(tcgetattr(fd_, &tty_) != 0){
        return APP::MsgReturn_t::ERROR_IN_OPEN;
    }

    tcflush(fd_, TCIOFLUSH);

    cfmakeraw(&tty_);

    tty_.c_cc[VMIN] = 8; //bloquea hasta recibir exactamente 8bytes
    tty_.c_cc[VTIME] = 1; //timeout de 0.1 segundos entre bytes

    if(tcsetattr(fd_, TCSANOW, &tty_) != 0){
        return APP::MsgReturn_t::ERROR_IN_OPEN;
    }

    state_ = true;

    return APP::MsgReturn_t::PORT_OPENED;
}

APP::MsgReturn_t SerialPortPsoc::close_port(){
    tcflush(fd_, TCIOFLUSH);
    int state;
    state = close(fd_);
    if(state != 0){
        return APP::MsgReturn_t::ERROR_IN_CLOSE;
    }
    state_ = false;
    return APP::MsgReturn_t::PORT_CLOSED;
}

APP::MsgReturn_t SerialPortPsoc::catch_data(void* userData){
    auto dataPt = reinterpret_cast<INFRA::SignalCapturer*>(userData);

    assert(dataPt != nullptr);

    assert(dataPt->get_voltages_ref() != nullptr);

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
                std::lock_guard<std::mutex> lock(dataPt->get_mutex());
                dataPt->get_voltages_ref()[0].ringBuffer_.push_data(&data1, 1);
                dataPt->get_voltages_ref()[1].ringBuffer_.push_data(&data2, 1);
                dataPt->get_voltages_ref()[2].ringBuffer_.push_data(&data3, 1);
                dataPt->get_voltages_ref()[3].ringBuffer_.push_data(&data4, 1);
            }
        }else{
            float cero = 0.0f;
            {
                std::lock_guard<std::mutex> lock(dataPt->get_mutex());
                dataPt->get_voltages_ref()[0].ringBuffer_.push_data(&cero, 1);
                dataPt->get_voltages_ref()[1].ringBuffer_.push_data(&cero, 1);
                dataPt->get_voltages_ref()[2].ringBuffer_.push_data(&cero, 1);
                dataPt->get_voltages_ref()[3].ringBuffer_.push_data(&cero, 1);
            }
        }
    }catch(...){
        return APP::MsgReturn_t::ERROR_IN_CATCH;
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
    return state_;
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
    state_ = false;
}

SerialPortPsoc::~SerialPortPsoc(){
    if(fd_ > 0){
        close_port();
        state_ = false;
    }
}

