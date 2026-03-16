#include <iostream>
#include <stdlib.h>

#include <utils/capturers/puertoSerie.hpp>
#include <utils/signal/voltageSignal.hpp>

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
SerialPort::SerialPort(){
}

SerialPort::~SerialPort(){
    close_port();
}

int SerialPort::open_port(const char* port){
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

int SerialPort::close_port(){
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

bool SerialPort::get_flag_serial(){
    return mySerial_.IsOpen();
}

IdCapturer_t SerialPort::get_Id(){
    return SERIAL_PORT_ID;
}

void SerialPort::read_values(VoltageSignal *volt1, VoltageSignal *volt2, VoltageSignal *volt3, VoltageSignal *volt4, unsigned int nValues){
    if(mySerial_.IsOpen()){
        char inputValues[32]; //32bytes: 4 canales, dos bytes por dato y nValues capturados.
        unsigned int offsetIndx = 0;
        while(offsetIndx < sizeof(inputValues)){
            try{
                mySerial_.read(inputValues + offsetIndx, sizeof(inputValues) - offsetIndx);
                std::streamsize got = mySerial_.gcount();
                if(got <= 0){
                    continue;
                }
                offsetIndx += static_cast<unsigned int>(got);
            }
            catch(std::exception &e){
                std::cerr << "No captura data" << e.what() << std::endl;
            }
        }

        unsigned int indexAux = 0;
        for(unsigned int i = volt1->length_-nValues; i < volt1->length_; i++){
            volt1->pivotVoltage_pt_[i] = static_cast<float>(inputValues[indexAux    ] | ((inputValues[indexAux + 1])<<8))*5.0f/4095.0f;
            volt2->pivotVoltage_pt_[i] = static_cast<float>(inputValues[indexAux + 2] | ((inputValues[indexAux + 3])<<8))*5.0f/4095.0f;
            volt3->pivotVoltage_pt_[i] = static_cast<float>(inputValues[indexAux + 4] | ((inputValues[indexAux + 5])<<8))*5.0f/4095.0f;
            volt4->pivotVoltage_pt_[i] = static_cast<float>(inputValues[indexAux + 6] | ((inputValues[indexAux + 7])<<8))*5.0f/4095.0f;
            indexAux += 8; //4 canales 2 bytes por dato.
        }
    }else{
        for(unsigned int i = volt1->length_-nValues; i < volt1->length_; i++){
            volt1->pivotVoltage_pt_[i] = 0.0f;
            volt2->pivotVoltage_pt_[i] = 0.0f;
            volt3->pivotVoltage_pt_[i] = 0.0f;
            volt4->pivotVoltage_pt_[i] = 0.0f;
        }
    }
}

void SerialPort::set_sample_frequency(unsigned int freq){
    char sendFreq[50];
    if(mySerial_.IsOpen()){
        sprintf(sendFreq, "%u\n", freq);
        //mySerial_.write(sendFreq, 2);
    }
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
bool SerialPort::start_with(std::string line, const char* text){
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
