#include <iostream>
#include "frameworksAndDrivers/serialPort/usbDevice.hpp"

using namespace DRV_FRAMW;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
UsbDevice::UsbDevice(): contextPtr_(nullptr), devHandlePtr_(nullptr){
}

UsbDevice::~UsbDevice(){
    close();
}

bool UsbDevice::open(uint16_t vid, uint16_t pid){
    /*std::cout << "this = " << this << std::endl;
    int status = libusb_init_context(&contextPtr_, nullptr, 0);
    std::cout << "status = " << status
          << " (" << libusb_error_name(status) << ")\n";

    std::cout << "contextPtr_ = " << contextPtr_ << '\n';
    if(status != LIBUSB_SUCCESS){
        std::cerr << "libusb_init " << libusb_error_name(status) << std::endl;
        close();
        return false;
    }

    devHandlePtr_ = libusb_open_device_with_vid_pid(contextPtr_, vid, pid);
    if(devHandlePtr_ == nullptr){
        std::cerr << "Device not found " << std::endl;
        close();
        return false;
    }
    
    #ifdef __linux__
        status = libusb_kernel_driver_active(devHandlePtr_,0);

        std::cout << "kernel_driver_active = "
              << status << std::endl;

        if(status == 1){
            status = libusb_detach_kernel_driver(devHandlePtr_,0);

            std::cout << "detach = "
                    << libusb_error_name(status)
                    << std::endl;
        }
    #endif

    status = libusb_claim_interface(devHandlePtr_, 0);
    std::cout << "claim_interface = "
              << libusb_error_name(status)
              << std::endl;
    if(status != 0){
        close();
        return false;
    }

    status = libusb_set_interface_alt_setting(devHandlePtr_, 0, 1);
    std::cout << "set_alt = "
              << libusb_error_name(status)
              << std::endl;
    if(status != 0){
        close();
        return false;
    }

    return true;*/

        std::cout << "this = " << this << std::endl;

        int status = libusb_init_context(&contextPtr_, nullptr, 0);
        std::cout << "status = "
                << status
                << " ("
                << libusb_error_name(status)
                << ")\n";

        std::cout << "contextPtr_ = "
                << contextPtr_
                << std::endl;

        if (status != LIBUSB_SUCCESS)
        {
            std::cerr << "libusb_init_context: "
                    << libusb_error_name(status)
                    << std::endl;

            close();
            return false;
        }

        libusb_device **list = nullptr;

        ssize_t count = libusb_get_device_list(contextPtr_, &list);

        if (count < 0)
        {
            std::cerr << "libusb_get_device_list: "
                    << libusb_error_name((int)count)
                    << std::endl;

            close();
            return false;
        }

        std::cout << "Dispositivos encontrados: "
                << count
                << std::endl;

        devHandlePtr_ = nullptr;

        for (ssize_t i = 0; i < count; ++i)
        {
            libusb_device_descriptor desc;

            status = libusb_get_device_descriptor(list[i], &desc);

            if (status != LIBUSB_SUCCESS)
                continue;

            std::cout << std::hex
                    << "VID:PID = "
                    << desc.idVendor
                    << ":"
                    << desc.idProduct
                    << std::dec
                    << std::endl;

            if (desc.idVendor == vid &&
                desc.idProduct == pid)
            {
                status = libusb_open(list[i], &devHandlePtr_);

                std::cout << "libusb_open = "
                        << libusb_error_name(status)
                        << std::endl;

                break;
            }
        }

        libusb_free_device_list(list, 1);

        if (devHandlePtr_ == nullptr)
        {
            std::cerr << "Device not found" << std::endl;
            close();
            return false;
        }

    #ifdef __linux__
        status = libusb_kernel_driver_active(devHandlePtr_, 0);

        std::cout << "kernel_driver_active = "
                << status
                << std::endl;

        if (status == 1)
        {
            status = libusb_detach_kernel_driver(devHandlePtr_, 0);

            std::cout << "detach = "
                    << libusb_error_name(status)
                    << std::endl;
        }
    #endif

        status = libusb_claim_interface(devHandlePtr_, 0);

        std::cout << "claim_interface = "
                << libusb_error_name(status)
                << std::endl;

        if (status != LIBUSB_SUCCESS)
        {
            close();
            return false;
        }

        status = libusb_set_interface_alt_setting(devHandlePtr_, 0, 1);

        std::cout << "set_alt = "
                << libusb_error_name(status)
                << std::endl;

        if (status != LIBUSB_SUCCESS)
        {
            close();
            return false;
        }

        return true;
}

void UsbDevice::close(){
    if(devHandlePtr_ != nullptr){
        libusb_release_interface(devHandlePtr_,0);
        libusb_close(devHandlePtr_);
        devHandlePtr_ = nullptr;
    }

    if(contextPtr_ != nullptr){
        libusb_exit(contextPtr_);
        contextPtr_ = nullptr;
    }
}

libusb_context* UsbDevice::get_context() const{
    return contextPtr_;
}

libusb_device_handle* UsbDevice::get_handle() const{
    return devHandlePtr_;
}
