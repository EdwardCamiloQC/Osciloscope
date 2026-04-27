/** \file serialPort.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date April 2026
 */
#pragma once

#include <termios.h>

#include <adapters/capturer.hpp>

namespace SERIAL_PORT{
    /** \class
     */
    class SerialPortPsoc: public Capturer{
        //====================
        // METHODS
        //====================
        public:
            /** \brief Constructor.
             */
            [[nodiscard]] SerialPortPsoc();

            /** \note Copy-constructor disabled.
             *  \param other: SerialPortPsoc object.
             */
            [[nodiscard]] SerialPortPsoc(const SerialPortPsoc &other) = delete;

            /** \note Move-Constructor disabled.
             *  \param other: SerialPortPsoc object.
             */
            [[nodiscard]] SerialPortPsoc(SerialPortPsoc &&other) = delete;

            /** \brief Destructor.
             */
            ~SerialPortPsoc();

            /** \note Assignment operator disabled.
             *  \param other: SerialPortPsoc object.
             *  \return SerialPortPsoc object.
             */
            [[nodiscard]]SerialPortPsoc& operator =(const SerialPortPsoc &other) = delete;
            [[nodiscard]]SerialPortPsoc& operator =(SerialPortPsoc &&other) = delete;

            /**
             * 
             */
            int open_port(const char* portName) override;

            /**
             * 
             */
            int close_port() override;

            /**
             * 
             */
            void catch_data(void* userData) override;

            /**
             * 
             */
            void set_data(void* userData) override;

            /**
             * 
             */
            bool get_flag_serial() override;

            /**
             * 
             */
            IdCapturer_t get_Id() override;

        //====================
        // ATTRIBUTES
        //====================
        private:
            int            fd_ {-1};
            struct termios tty_;
            bool           state_;
    };
}
