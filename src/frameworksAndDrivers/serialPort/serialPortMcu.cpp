#include <stdlib.h>
#include <cstring>
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

void SerialPortAnyMcu::associate_screen(APP::IScreen* screenPtr){
    screenPtr_ = screenPtr; 
}

int SerialPortAnyMcu::open_port(const char* port){
    if(!(port)){
        if(screenPtr_ != nullptr)
            screenPtr_->set_message("No hay nombre de puerto serie para abrir.\n", 2);
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
                if(screenPtr_ != nullptr){
                    screenPtr_->set_message("Serial Abierto: ", 4);
                    screenPtr_->set_message(port, 4);
                    screenPtr_->set_message("\n", 4);
                    screenPtr_->update_port_state(true);
                }
                return EXIT_SUCCESS;
            }
        }
        catch(std::exception& e){
            if(screenPtr_ != nullptr){
                screenPtr_->set_message("Error al abrir serial: ", 2);
                screenPtr_->set_message(port, 2);
                screenPtr_->set_message(e.what(), 2);
                screenPtr_->set_message("\n", 2);
            }
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
                if(screenPtr_ != nullptr){
                    screenPtr_->set_message("Serial Cerrado\n", 5);
                    screenPtr_->update_port_state(false);
                }
                return EXIT_SUCCESS;
            }
        }catch(std::exception& e){
            screenPtr_->set_message("Error al cerrar el puerto serie: ", 2);
            screenPtr_->set_message(e.what(), 2);
            screenPtr_->set_message("\n", 2);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

void SerialPortAnyMcu::catch_data(void* userData){
    auto dataPt = reinterpret_cast<INFRA::SignalCapturer*>(userData);

    if(dataPt == nullptr){
        screenPtr_->set_message("No se asoció un SignalCapturer para tratar los datos.", 2);
        return;
    }

    if(dataPt->get_voltages_ref() == nullptr){
        screenPtr_->set_message("No hay voltajes ascociados al SignalCapturer.", 2);
        return;
    }

    uint16_t data1, data2, data3, data4;

    if(mySerial_.IsOpen()){
        std::string line;
        char inputValues[32]; //32bytes: 4 canales, dos bytes por dato y nValues capturados.
        try{
            std::getline(mySerial_, line);
        }
        catch(std::exception &e){
            screenPtr_->set_message("No captura data", 2);
            screenPtr_->set_message(e.what(), 2);
            screenPtr_->set_message("\n", 2);
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
}

void SerialPortAnyMcu::set_data([[maybe_unused]]void* userData){
    if(userData == nullptr){
        screenPtr_->set_message("No se asociaron datos para enviar.\n", 2);
        return;
    }
    //char sendFreq[50];
    //if(mySerial_.IsOpen()){
        //sprintf(sendFreq, "%u\n", freq);
        //mySerial_.write(sendFreq, 2);
    //}
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
