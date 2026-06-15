#include <fstream>
#include <string.h>
#include <assert.h>
#include <mutex>
#include "frameworksAndDrivers/persistence/systemFile/docGenerator.hpp"
#include "domain/signals/voltageSignal.hpp"
#include "infrastructure/adapters/screen.hpp"
#include "infrastructure/adapters/signalCapturer.hpp"

using namespace DRV_FRAMW;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
void DocGenerator::associate_voltages(DOMN::VoltageSignal* volts){
    voltsPtr_ = volts;
}

void DocGenerator::associate_screen(APP::IScreen* screenPtr){
    screenPtr_ = screenPtr;
}

void DocGenerator::associate_signal_capturer(APP::ISignalCapturer* sigCapPtr){
    signalCapPtr_ = sigCapPtr;
}

void DocGenerator::generate_doc(const char *directory, APP::TYPE_DOC type, float timeDiv) const{
    assert(screenPtr_ != nullptr);
    assert(signalCapPtr_ != nullptr);

    std::fstream archivo;
    std::string ruta = directory;

    switch(type){
        case APP::TYPE_DOC::CSV:
            ruta.append("voltajes.csv");
            break;
        case APP::TYPE_DOC::JSON:
            ruta.append("voltajes.json");
            break;
        case APP::TYPE_DOC::TSV:
            ruta.append("voltajes.tsv");
            break;
        case APP::TYPE_DOC::TXT:
            ruta.append("voltajes.txt");
            break;
    }

    archivo.open(ruta, std::ios_base::out);

    if(archivo.is_open()){
        archivo << "fs = " << timeDiv << "Hz\n\n";

        for(int s = 0; s < 4; s++){
            archivo << "Señal: " << s << std::endl;
            for(size_t i = 0; i < voltsPtr_[0].ringBuffer_.get_lenght(); i++){
                {
                    std::lock_guard<std::mutex> lock(signalCapPtr_->get_mutex());
                    archivo << *voltsPtr_[s].ringBuffer_.get_ring_buffer_ref(i) << "V\n";
                }
            }
            archivo.write("\n",1);
        }
        screenPtr_->set_message("Archivo Generado.\n", 4);
    }else{
        perror("Error");
        return;
    }

    archivo.close();
}
