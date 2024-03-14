#include <puertoSerie.h>
#include <iostream>

ComSerial::ComSerial(const char* port){
    flagOpen = false;
    initSignalsToZero();
    assignColorToSignals();
    assignElements();
    openPort(port);
}

void ComSerial::run(unsigned short scroll){
    scrollSignals(scroll);
    for(size_t i = 20; i > 0; i--){
        if(mySerial.IsOpen()){
            std::string line;
            std::getline(mySerial, line);
            if(startWith(line, "#")){
                sscanf(line.c_str(), "#%x@%x/n", &signal1, &signal2);
                voltage = static_cast<float>(signal1) / 65535;
                dataVoltage1[sizeof(dataVoltage1)/sizeof(float)+1-2*i] = voltage;
                voltage = static_cast<float>(signal2) / 65535;
                dataVoltage2[sizeof(dataVoltage2)/sizeof(float)+1-2*i] = voltage;
            }
        }else{
            dataVoltage1[sizeof(dataVoltage1)/sizeof(float)+1-2*i] = 0.0f;
            dataVoltage2[sizeof(dataVoltage2)/sizeof(float)+1-2*i] = 0.0f;
        }
    }
}

void ComSerial::initSignalsToZero(){
    for(size_t  i = 0; i < sizeof(dataVoltage1)/sizeof(float); i++){
        if(i%2 == 0){
            if(i == 0){
                dataVoltage1[i] = -1.0f;
                dataVoltage2[i] = -1.0f;
            }else{
                dataVoltage1[i] = dataVoltage1[i-2] + static_cast<float>(4)*static_cast<float>(sizeof(float))/static_cast<float>(sizeof(dataVoltage1));
                dataVoltage2[i] = dataVoltage2[i-2] + static_cast<float>(4)*static_cast<float>(sizeof(float))/static_cast<float>(sizeof(dataVoltage2));
            }
        }else{
            dataVoltage1[i] = 0.0f;
            dataVoltage2[i] = 0.0f;
        }
    }
}

void ComSerial::assignColorToSignals(){
    for(size_t i = 0; i < sizeof(colorSignal1)/sizeof(float)-2; i+=3){
        colorSignal1[i] = 1.0f;
        colorSignal1[i+1] = 0.0f;
        colorSignal1[i+2] = 0.0f;
        colorSignal2[i] = 0.0f;
        colorSignal2[i+1] = 1.0f;
        colorSignal2[i+2] = 0.0f;
    }
}

void ComSerial::openPort(const char* port){
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

void ComSerial::scrollSignals(unsigned short landslide){
    for(size_t i = 1; i < (sizeof(dataVoltage1)/sizeof(float))-1; i+=2){
        dataVoltage1[i] = dataVoltage1[i+2*landslide];
        dataVoltage2[i] = dataVoltage2[i+2*landslide];
    }
}

void ComSerial::assignElements(){
    elementSignals[0] = 0;
    for(size_t i = 1; i < sizeof(elementSignals)/sizeof(unsigned int)-2; i+=2){
        elementSignals[i] = i;
        elementSignals[i+1] = i;
    }
    elementSignals[sizeof(elementSignals)/sizeof(unsigned int)-1] = (sizeof(dataVoltage1)/sizeof(float))/2-1;
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
