#include <iostream>
#include <stdlib.h>
#include <cstring>

#include <infrastructure/serialPort/serialPortMcu.hpp>
#include <application/oscilloscope.hpp>

using namespace SERIAL_PORT;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
SerialPortAnyMcu::SerialPortAnyMcu(){
}

SerialPortAnyMcu::~SerialPortAnyMcu(){
    close_port();
    std::cout << "Destructor any MCU\n";
}

int SerialPortAnyMcu::open_port(const char* port){
    if(!(port)){
        return EXIT_FAILURE;
    }
    if(!mySerial_.IsOpen()){
        try{
            mySerial_.Open(port);
            if(!mySerial_.fail()){
                mySerial_.SetBaudRate(LibSerial::BaudRate::BAUD_115200);
                mySerial_.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
                mySerial_.SetStopBits(LibSerial::StopBits::STOP_BITS_1);
                mySerial_.SetParity(LibSerial::Parity::PARITY_NONE);
                mySerial_.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
                std::cout << "Serial Abierto: " << port << std::endl;
                return EXIT_SUCCESS;
            }
        }
        catch(std::exception& e){
            std::cerr << "Error al abrir serial: " << port << "-->" << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int SerialPortAnyMcu::close_port(){
    if(mySerial_.IsOpen()){
        try{
            mySerial_.Close();
            if(!mySerial_.IsOpen()){
                std::cout << "Serial Cerrado\n";
                return EXIT_SUCCESS;
            }
        }catch(std::exception& e){
            std::cerr << "Error al cerrar el puerto serie: " << "-->" << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

void SerialPortAnyMcu::catch_data(void* userData){
    OSCILLOSCOPE::Oscilloscope* oscPt = reinterpret_cast<OSCILLOSCOPE::Oscilloscope*>(userData);

    uint16_t data1, data2, data3, data4;

    if(mySerial_.IsOpen()){
        std::string line;
        char inputValues[32]; //32bytes: 4 canales, dos bytes por dato y nValues capturados.
        try{
            std::getline(mySerial_, line);
        }
        catch(std::exception &e){
            std::cerr << "No captura data" << e.what() << std::endl;
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
                std::lock_guard<std::mutex> lock(oscPt->mutex_);
                oscPt->voltage1_.ringBuffer_.push_data(&d1, 1);
                oscPt->voltage2_.ringBuffer_.push_data(&d2, 1);
                oscPt->voltage3_.ringBuffer_.push_data(&d3, 1);
                oscPt->voltage4_.ringBuffer_.push_data(&d4, 1);
            }
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
}

void SerialPortAnyMcu::set_data([[maybe_unused]]void* userData){
    //char sendFreq[50];
    //if(mySerial_.IsOpen()){
        //sprintf(sendFreq, "%u\n", freq);
        //mySerial_.write(sendFreq, 2);
    //}
}

bool SerialPortAnyMcu::get_flag_serial(){
    return mySerial_.IsOpen();
}

IdCapturer_t SerialPortAnyMcu::get_Id(){
    return IdCapturer_t::SERIAL_PORT_ID;
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
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
