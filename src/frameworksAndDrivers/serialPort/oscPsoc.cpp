#include <iostream>
#include <assert.h>
#include <iomanip>
#include "frameworksAndDrivers/serialPort/oscPsoc.hpp"
#include "infrastructure/adapters/screen.hpp"
#include "infrastructure/adapters/signalCapturer.hpp"
#include "domain/signals/voltageSignal.hpp"

using namespace DRV_FRAMW;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
OscPsoc& OscPsoc::get_instance(){
    static OscPsoc instance;
    return instance;
}

APP::MsgReturn_t OscPsoc::start(INFRA::SignalCapturer* sigCapPt, [[maybe_unused]]const char* port){
    if(!UsbDevice::open(0x4B4, 0x8051)){
        std::cout << "No se pudo abrir el dispositivo\n";
        return APP::MsgReturn_t::ERROR_IN_OPEN;
    }

    transferPtr_ = libusb_alloc_transfer(NUM_PACKETS);
    if(transferPtr_ == nullptr){
        UsbDevice::close();
        return APP::MsgReturn_t::ERROR_IN_OPEN;
    }

    libusb_fill_iso_transfer(transferPtr_, UsbDevice::get_handle(), 0x81, buffer_.data(), buffer_.size(), NUM_PACKETS, callback, sigCapPt, 0);

    libusb_set_iso_packet_lengths(transferPtr_, PACKET_SIZE);

    int status = libusb_submit_transfer(transferPtr_);
    if(status != LIBUSB_SUCCESS){
        libusb_free_transfer(transferPtr_);
        transferPtr_ = nullptr;
        UsbDevice::close();
        return APP::MsgReturn_t::ERROR_IN_OPEN;
    }

   return APP::MsgReturn_t::PORT_OPENED;
}

APP::MsgReturn_t OscPsoc::stop(){
    if(transferPtr_ == nullptr)
        return APP::MsgReturn_t::ERROR_IN_CLOSE;

    int status = libusb_cancel_transfer(transferPtr_);

    if(status != LIBUSB_SUCCESS && status != LIBUSB_ERROR_NOT_FOUND){
        std::cerr << libusb_error_name(status) << '\n';
        return APP::MsgReturn_t::ERROR_IN_CLOSE;
    }

    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    while(transferPtr_ != nullptr){
        int status = libusb_handle_events_timeout_completed(UsbDevice::get_context(), &tv, nullptr);

        if(status != LIBUSB_SUCCESS)
            break;
    }

    UsbDevice::close();
    return APP::MsgReturn_t::PORT_CLOSED;
}

APP::MsgReturn_t OscPsoc::catch_data(void* userData){
    auto dataPt = reinterpret_cast<INFRA::SignalCapturer*>(userData);
    assert(dataPt != nullptr);
    assert(dataPt->get_voltages_ref() != nullptr);

    if(get_flag_serial()){
        libusb_handle_events(UsbDevice::get_context());
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

APP::MsgReturn_t OscPsoc::set_data([[maybe_unused]]void* userData){
    return APP::MsgReturn_t::SEND;
}

bool OscPsoc::get_flag_serial(){
    return get_context() != nullptr && get_handle()!= nullptr;
}

APP::IdCapturer_t OscPsoc::get_Id(){
    return APP::IdCapturer_t::USB_PSOC_ID;
}
//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
OscPsoc::OscPsoc(): UsbDevice(){
}

OscPsoc::~OscPsoc(){
    if(transferPtr_ != nullptr)
        stop();
}

void OscPsoc::callback(libusb_transfer* transfer){
    auto dataPt = static_cast<INFRA::SignalCapturer*>(transfer->user_data);
    assert(dataPt != nullptr);
    assert(dataPt->get_voltages_ref() != nullptr);

    switch(transfer->status){
        case LIBUSB_TRANSFER_COMPLETED:
            break;
        case LIBUSB_TRANSFER_ERROR:
        case LIBUSB_TRANSFER_TIMED_OUT:
        case LIBUSB_TRANSFER_STALL:
        case LIBUSB_TRANSFER_OVERFLOW:
            if(libusb_submit_transfer(transfer) != LIBUSB_SUCCESS){
                libusb_free_transfer(transfer);
                OscPsoc::get_instance().transferPtr_ = nullptr;
            }
            return;
        case LIBUSB_TRANSFER_NO_DEVICE:
        case LIBUSB_TRANSFER_CANCELLED:
            {
                libusb_free_transfer(transfer);
                OscPsoc::get_instance().transferPtr_ = nullptr;
                return;
            }
        default:
            std::cerr << "Transfer error: "
                << libusb_error_name(transfer->status)
                << std::endl;

            libusb_submit_transfer(transfer);
            return;
    }

    for(uint32_t i = 0; i < NUM_PACKETS; i++){
        auto& packet = transfer->iso_packet_desc[i];

        if(packet.status != LIBUSB_TRANSFER_COMPLETED)
            continue;

        if(packet.actual_length == 0)
            continue;

        unsigned char header = buffer_[0];
        size_t len = packet.actual_length - 2;
        if(header == 0xFF){
            std::lock_guard<std::mutex> lock(dataPt->get_mutex());
            dataPt->get_voltages_ref()[0].ringBuffer_.push_data(&buffer_[2], len, 8, 5.0f);
        }

        /*unsigned char* ptr = libusb_get_iso_packet_buffer_simple(transfer, i);

        std::cout << "Ancho:" << packet.actual_length << " "
            << "Pack:" << i << " "
            << std::hex
            << "Bytes ";
            for(int m = 0; m < 16; m++){
                std::cout << std::setw(1) << static_cast<unsigned>(ptr[m]) << " ";
            }
            std::cout << " | ";
            for(int m = 496; m <512; m++){
                std::cout << std::setw(1) << static_cast<unsigned>(ptr[m]) << " ";
            }*/
    }

    //std::cout << std::dec << std::endl; //Código comentado (verifica el tráfico de datos que llegan.)

    int status = libusb_submit_transfer(transfer);

    if(status != LIBUSB_SUCCESS){
        std::cerr << "Error al reenviar transferencia: "
            << libusb_error_name(status) << std::endl;

        libusb_free_transfer(transfer);

        OscPsoc::get_instance().transferPtr_ = nullptr;
    }
}
