/** \file usbDevice.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date July 2026
 */
#pragma once

#include <stdint.h>
#include <libusb-1.0/libusb.h>

/** \namespace DRV_FRAMW
 */
namespace DRV_FRAMW{
    /** \class UsbDevice.
     */
    class UsbDevice{
        //====================
        // METHODS
        //====================
        public:
            /** \brief Principal-Constructor.
             */
            [[nodiscard]] UsbDevice();

            /** \note Copy-Constructor disabled.
             *  \param other: UsbDevice object.
             */
            UsbDevice(const UsbDevice& other) = delete;

            /** \note Move-Constructor disabled.
             *  \param other: UsbObject disabled.
             */
            UsbDevice(UsbDevice&& other) = delete;

            /** \brief Destructor.
             */
            ~UsbDevice();

            /** \note Assignment operator disabled.
             *  \param other: UsbDevice object.
             *  \return UsbDevice object.
             */
            UsbDevice& operator =(const UsbDevice& other) = delete;
            UsbDevice& operator =(UsbDevice&& other) = delete;

            /** \brief Open USB device.
             *  \param vid: Vendor Id.
             *  \param pid: Product Id.
             *  \return Status.
             */
            bool open(uint16_t vid, uint16_t pid);

            /** \brief Close the USB device.
             */
            void close();

            /** \brief Return the device context.
             *  \return Context.
             */
            libusb_context* get_context() const;

            /** \brief Return the handle.
             *  \return Handle.
             */
            libusb_device_handle* get_handle() const;
        //====================
        // ATTRIBUTES
        //====================
        private:
            libusb_context*       contextPtr_   {nullptr};
            libusb_device_handle* devHandlePtr_ {nullptr};
    };
}
