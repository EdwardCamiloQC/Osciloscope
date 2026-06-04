#include <stdlib.h>
#include <cstring>
#include <assert.h>
#include "frameworksAndDrivers/serialPort/serialPortMcu.hpp"
#include "infrastructure/adapters/screen.hpp"
#include "infrastructure/adapters/signalCapturer.hpp"
#include "domain/signals/voltageSignal.hpp"

using namespace DRV_FRAMW;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
SerialPortAnyMcu& SerialPortAnyMcu::get_instance(){
    static SerialPortAnyMcu instance;
    return instance;
}

APP::MsgReturn_t SerialPortAnyMcu::open_port(const char* port){
    if(!(port))
        return APP::MsgReturn_t::DONT_NAME_PORT;

    if(!mySerial_.IsOpen()){
        try{
            mySerial_.Open(port);
            if(!mySerial_.fail()){
                mySerial_.SetBaudRate(LibSerial::BaudRate::BAUD_115200);
                mySerial_.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
                mySerial_.SetStopBits(LibSerial::StopBits::STOP_BITS_1);
                mySerial_.SetParity(LibSerial::Parity::PARITY_NONE);
                mySerial_.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
                return APP::MsgReturn_t::PORT_OPENED;
            }
        }
        catch(std::exception& e){
            return APP::MsgReturn_t::ERROR_IN_OPEN;
        }
    }
    return APP::MsgReturn_t::PORT_OPENED;
}

APP::MsgReturn_t SerialPortAnyMcu::close_port(){
    if(mySerial_.IsOpen()){
        try{
            mySerial_.Close();
            if(!mySerial_.IsOpen()){
                return APP::MsgReturn_t::PORT_CLOSED;
            }
        }catch(std::exception& e){
            return APP::MsgReturn_t::ERROR_IN_CLOSE;
        }
    }
    return APP::MsgReturn_t::PORT_CLOSED;
}

APP::MsgReturn_t SerialPortAnyMcu::catch_data(void* userData){
    auto dataPt = reinterpret_cast<INFRA::SignalCapturer*>(userData);

    assert(dataPt != nullptr);

    assert(dataPt->get_voltages_ref() != nullptr);

    uint16_t data1, data2, data3, data4;

    if(mySerial_.IsOpen()){
        std::string line;
        char inputValues[32]; //32bytes: 4 canales, dos bytes por dato y nValues capturados.
        try{
            std::getline(mySerial_, line);
        }
        catch(std::exception &e){
            return APP::MsgReturn_t::ERROR_IN_CATCH;
        }

        if(start_with(line, "#")){
            std::strcpy(inputValues, line.c_str());
            sscanf(inputValues, "#%hu@%hu@%hu@%hu\n\r", &data1, &data2, &data3, &data4);
            float d1, d2, d3, d4;
            d1 = (static_cast<float>(data1)*5.0f/4095.0f) - 5.0f;
            d2 = (static_cast<float>(data2)*5.0f/4095.0f) - 5.0f;
            d3 = (static_cast<float>(data3)*5.0f/4095.0f) - 5.0f;
            d4 = (static_cast<float>(data4)*5.0f/4095.0f) - 5.0f;
            {
                std::lock_guard<std::mutex> lock(dataPt->get_mutex());
                dataPt->get_voltages_ref()[0].ringBuffer_.push_data(&d1, 1);
                dataPt->get_voltages_ref()[1].ringBuffer_.push_data(&d2, 1);
                dataPt->get_voltages_ref()[2].ringBuffer_.push_data(&d3, 1);
                dataPt->get_voltages_ref()[3].ringBuffer_.push_data(&d4, 1);
            }
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
    return APP::MsgReturn_t::CATCH;
}

APP::MsgReturn_t SerialPortAnyMcu::set_data([[maybe_unused]]void* userData){
    if(userData == nullptr){
        return APP::MsgReturn_t::ERROR_IN_SEND;
    }
    //char sendFreq[50];
    //if(mySerial_.IsOpen()){
        //sprintf(sendFreq, "%u\n", freq);
        //mySerial_.write(sendFreq, 2);
    //}
    return APP::MsgReturn_t::SEND;
}

bool SerialPortAnyMcu::get_flag_serial(){
    return mySerial_.IsOpen();
}

APP::IdCapturer_t SerialPortAnyMcu::get_Id(){
    return APP::IdCapturer_t::SERIAL_PORT_ANY_ID;
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
SerialPortAnyMcu::SerialPortAnyMcu(){
}

SerialPortAnyMcu::~SerialPortAnyMcu(){
    close_port();
}

bool SerialPortAnyMcu::start_with(std::string line, const char* text){
    std::string aux = text;
    size_t textLen = aux.length();
    if(line.size() < textLen){
        return false;
    }
    for(size_t i = 0; i < line.size(); i++){
        if(line[i] == text[0]){
            return true;
        }   
    }
    return false;
}
