#include <puertoSerie.h>
#include <iostream>

ComSerial::ComSerial(const char* port){
    flagOpen = false;
    float auxAxeX = -1.0f;
    prueba[0] = -0.5f;  prueba[1] = 0.0f;
    prueba[2] = 0.0f;   prueba[3] = 0.5f;
    prueba[4] = 0.5f;   prueba[5] = 0.0f;

    for(size_t  i = 1; i < sizeof(dataVoltage)/sizeof(float); i+=2){
        dataVoltage[i] = 0.0f;
        dataVoltage[i-1] = auxAxeX + static_cast<float>(2)/static_cast<float>(sizeof(dataVoltage));
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
    for(size_t i = 1; i < sizeof(dataVoltage)/sizeof(float); i+=2){
        if(mySerial.IsOpen()){
            std::string line;
            std::getline(mySerial, line);
            if(startWith(line, "#")){
                sscanf(line.c_str(), "#%x@/r/n", &valueADC);
                voltage = static_cast<float>(valueADC) / 65535;
                dataVoltage[i] = voltage;
                std::cout << valueADC << "..." << voltage << std::endl;
            }
        }
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
