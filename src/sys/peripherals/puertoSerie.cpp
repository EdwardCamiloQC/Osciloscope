#include <sys/peripherals/puertoSerie.hpp>
#include <libraries/signal/voltageSignal.hpp>
#include <iostream>
#include <stdlib.h>

//----------
//      PUBLIC METHODS
//----------
ComSerial::ComSerial(VoltageSignal *volt1, VoltageSignal *volt2, VoltageSignal *volt3, VoltageSignal *volt4){
    voltage1 = volt1;
    voltage2 = volt2;
    voltage3 = volt3;
    voltage4 = volt4;
}

ComSerial::~ComSerial(){
    closePort();
}

bool ComSerial::openPort(const char* port){
    if(!mySerial.IsOpen()){
        try{
            mySerial.Open(port);
            if(!mySerial.fail()){
                mySerial.SetBaudRate(LibSerial::BaudRate::BAUD_115200);
                mySerial.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
                mySerial.SetStopBits(LibSerial::StopBits::STOP_BITS_1);
                mySerial.SetParity(LibSerial::Parity::PARITY_NONE);
                mySerial.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
            }
        }
        catch(std::exception& e){
            std::cerr << "error al abrir serial" << e.what() << std::endl;
        }
        return true;
    }
    return false;
}

bool ComSerial::closePort(){
    if(mySerial.IsOpen()){
        try{
            mySerial.Close();
            if(!mySerial.IsOpen()){
                std::cout << "Serial Cerrado\n";
            }
        }catch(std::exception& e){
            std::cerr << "Error al cerrar el puerto serie" << e.what() << std::endl;
        }
        return true;
    }
    return false;
}

bool ComSerial::getFlagSerial(){
    return mySerial.IsOpen();
}

void ComSerial::readValues(unsigned int nValues){
    voltage1->shiftVoltage(nValues);
    voltage2->shiftVoltage(nValues);
    voltage3->shiftVoltage(nValues);
    voltage4->shiftVoltage(nValues);
    if(mySerial.IsOpen()){
        unsigned int signal1{0}, signal2{0}, signal3{0}, signal4{0};
        std::string line;
        std::getline(mySerial, line);
        for(unsigned int i = voltage1->length-nValues; i < voltage1->length; i++){
            try{
                if(startWith(line, "#")){
                    sscanf(line.c_str(), "#@%x@%x@%x@%x/n", &signal1, &signal2, &signal3, &signal4);
                    voltage1->voltage[i] = static_cast<float>(signal1)*3.3f/65535.0f;
                    voltage2->voltage[i] = static_cast<float>(signal2)*3.3f/65535.0f;
                    voltage3->voltage[i] = static_cast<float>(signal3)*3.3f/65535.0f;
                    voltage4->voltage[i] = static_cast<float>(signal4)*3.3f/65535.0f;
                }
            }
            catch(std::exception &e){
                std::cerr << "No captura data" << e.what() << std::endl;
            }
        }
    }else{
        for(unsigned int i = voltage1->length-nValues; i < voltage1->length; i++){
            voltage1->voltage[i] = 0.0f;
            voltage2->voltage[i] = 0.0f;
            voltage3->voltage[i] = 0.0f;
            voltage4->voltage[i] = 0.0f;
        }
    }
}

void ComSerial::setSampleFrequency(unsigned int freq){
    char sendFreq[50];
    if(mySerial.IsOpen()){
        sprintf(sendFreq, "v\n");
        //mySerial.write(sendFreq, 2);
    }
}

//----------
//      PRIVATE METHODS
//----------
bool ComSerial::startWith(std::string line, const char* text){
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
