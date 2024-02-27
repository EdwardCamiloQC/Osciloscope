#include <puertoSerie.h>
#include <iostream>

ComSerial::ComSerial(const char* port){
    flagOpen = false;

    for(size_t  i = 0; i < sizeof(dataVoltage1)/sizeof(float); i++){
        if(i%2 == 0){
            if(i == 0){
                dataVoltage1[i] = -1.0f;
            }else{
                dataVoltage1[i] = dataVoltage1[i-2] + static_cast<float>(4)*static_cast<float>(sizeof(float))/static_cast<float>(sizeof(dataVoltage1));
            }
        }else{
            dataVoltage1[i] = 0.0f;
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
    scrollSignal(1);
    for(size_t i = 20; i > 0; i--){
        if(mySerial.IsOpen()){
            std::string line;
            std::getline(mySerial, line);
            if(startWith(line, "#")){
                sscanf(line.c_str(), "#%x@%x/n", &signal1, &signal2);
                voltage = static_cast<float>(signal1) / 65535;
                dataVoltage1[sizeof(dataVoltage1)/sizeof(float)+1-2*i] = voltage;
                std::cout << signal1 << "..." << voltage << std::endl;
            }
        }else{
            dataVoltage1[sizeof(dataVoltage1)/sizeof(float)+1-2*i] = 0.0f;
        }
    }
}

void ComSerial::scrollSignal(unsigned short landslide){
    for(size_t i = 1; i < (sizeof(dataVoltage1)/sizeof(float))-1; i+=2){
        dataVoltage1[i] = dataVoltage1[i+2*landslide];
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
