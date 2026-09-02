/** \file oscPsoc.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date July 2026
 */
#pragma once

#include <array>
#include "application/ICapturer.hpp"
#include "frameworksAndDrivers/serialPort/usbDevice.hpp"

/** \namespace DRV_FRAMW.
 */
namespace DRV_FRAMW{
    /** \class
     */
    class OscPsoc: public UsbDevice, public APP::ICapturer{
        //====================
        // METHODS
        //====================
        public:
            /** \brief Return the OscPsoc object.
             *  \return OscPsoc object.
             */
            static OscPsoc& get_instance();

            /** \brief Start the transfer.
             *  \return Status.
             */
            APP::MsgReturn_t start(INFRA::SignalCapturer* sigCapPt, const char* port) override final;

            /** \brief Stop the transfer.
             */
            APP::MsgReturn_t stop() override final;

            /**
             * 
             */
            APP::MsgReturn_t catch_data(void* userData) override final;

            /**
             * 
             */
            APP::MsgReturn_t set_data(void* userData) override final;

            /**
             * 
             */
            bool get_flag_serial() override final;

            /**
             * 
             */
            APP::IdCapturer_t get_Id() override final;
        private:
            /** \note Principal-Constructor private.
             */
            [[nodiscard]] OscPsoc();

            /** \note Copy-constructor disabled.
             *  \param other: OscPsoc object.
             */
            OscPsoc(const OscPsoc &other) = delete;

            /** \note Move-Constructor disabled.
             *  \param other: OscPsoc object.
             */
            OscPsoc(OscPsoc &&other) = delete;

            /** \note Destructor private.
             */
            ~OscPsoc();

            /** \note Assignment operator disabled.
             *  \param other: SerialPortPsoc object.
             *  \return SerialPortPsoc object.
             */
            OscPsoc& operator =(const OscPsoc &other) = delete;
            OscPsoc& operator =(OscPsoc &&other) = delete;

            /** \brief
             *  \param transfer:
             */
            static void callback(libusb_transfer* transfer);
        //====================
        // ATTRIBUTES
        //====================
        public:
            static constexpr uint32_t PACKET_SIZE {512};
            static constexpr uint32_t NUM_PACKETS {1};
            static constexpr uint32_t BUFFER_SIZE {PACKET_SIZE * NUM_PACKETS};
        private:
            inline static libusb_transfer*    transferPtr_ {nullptr};
            inline static std::array<uint8_t, BUFFER_SIZE> buffer_;
    };
}
