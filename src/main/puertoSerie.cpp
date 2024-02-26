#include <puertoSerie.h>
#include <iostream>

ComSerial::ComSerial(const char* port){
    flagOpen = false;

    for(size_t  i = 0; i < sizeof(dataVoltage)/sizeof(float); i++){
        if(i%2 == 0){
            if(i == 0){
                dataVoltage[i] = -1.0f;
            }else{
                dataVoltage[i] = dataVoltage[i-2] + static_cast<float>(4)*static_cast<float>(sizeof(float))/static_cast<float>(sizeof(dataVoltage));
            }
        }else{
            dataVoltage[i] = 0.0f;
        }
    }
    try{
        mySerial.Open(port);
        if(mySerial.fail()){
            throw 'f';
        }else{
            flagOpen = true;
            mySerial.SetBaudRate(LibSerial::BaudRate::BAUD_115200);
            mySerial.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
            mySerial.SetStopBits(LibSerial::StopBits::STOP_BITS_1);
            mySerial.SetParity(LibSerial::Parity::PARITY_NONE);
            mySerial.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
        }
    }
    catch(char f){
        std::cout << "error al abrir serial" << std::endl;
    }
    catch(std::exception& e){
        std::cout << "error al abrir serial" << std::endl;
        //throw std::ios::failure(e.what());
    }
}

void ComSerial::run(){
    scrollSignal();
    if(mySerial.IsOpen()){
        std::string line;
        std::getline(mySerial, line);
        if(startWith(line, "#")){
            sscanf(line.c_str(), "#%x@/r/n", &valueADC);
            voltage = static_cast<float>(valueADC) / 65535;
            dataVoltage[sizeof(dataVoltage)/sizeof(float)-1] = voltage;
            std::cout << valueADC << "..." << voltage << std::endl;
        }
    }else{
        dataVoltage[sizeof(dataVoltage)/sizeof(float)-1] = 0.0f;
    }
}

void ComSerial::scrollSignal(){
    for(size_t i = 1; i < (sizeof(dataVoltage)/sizeof(float))-1; i+=2){
        dataVoltage[i] = dataVoltage[i+2];
    }
}

ComSerial::~ComSerial(){
    if(flagOpen){
        mySerial.Close();
        flagOpen = false;
    }
}

bool startWith(std::string line, const char* text){
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
