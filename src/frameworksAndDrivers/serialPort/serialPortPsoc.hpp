/** \file serialPort.hpp
 *  \author Edward Camilo
 *  \version v2.0
 *  \date April 2026
 */
#pragma once

#include <termios.h>
#include "application/ICapturer.hpp"

struct IScreen;

namespace DRV_FRAMW{
    /** \class
     */
    class SerialPortPsoc: public APP::ICapturer{
        //====================
        // METHODS
        //====================
        public:
            /** \brief Obtains the instance.
             *  \return SerialPortPsoc object.
             */
            static SerialPortPsoc& get_instance();

            /** \brief Open serial port.
             *  \param portName: Port name.
             *  \return State serial port.
             */
            APP::MsgReturn_t open_port(const char* portName) override final;

            /** \brief Close serial port.
             *  \return State serial port.
             */
            APP::MsgReturn_t close_port() override final;

            /** \brief Reads the data in file TTY.
             *  \param userData: Pointer using to save the data.
             */
            APP::MsgReturn_t catch_data(void* userData) override final;

            /** \brief Sends data.
             *  \param userData: Data to send.
             */
            APP::MsgReturn_t set_data(void* userData) override final;

            /** \brief State of the serial port.
             *  \return State serial port.
             */
            bool get_flag_serial() override final;

            /** \brief Id to correspond to PSOC.
             *  \return Id.
             */
            APP::IdCapturer_t get_Id() override final;
        private:
            /** \brief Principal-Constructor.
             */
            [[nodiscard]] SerialPortPsoc();

            /** \note Copy-constructor disabled.
             *  \param other: SerialPortPsoc object.
             */
            SerialPortPsoc(const SerialPortPsoc &other) = delete;

            /** \note Move-Constructor disabled.
             *  \param other: SerialPortPsoc object.
             */
            SerialPortPsoc(SerialPortPsoc &&other) = delete;

            /** \brief Destructor.
             */
            ~SerialPortPsoc();

            /** \note Assignment operator disabled.
             *  \param other: SerialPortPsoc object.
             *  \return SerialPortPsoc object.
             */
            SerialPortPsoc& operator =(const SerialPortPsoc &other) = delete;
            SerialPortPsoc& operator =(SerialPortPsoc &&other) = delete;
        //====================
        // ATTRIBUTES
        //====================
        private:
            int            fd_ {-1};
            struct termios tty_;
    };
}
