/** \file serialPort.hpp
 *  \author Edward Camilo
 *  \version v1.0
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
            /** \brief Constructor.
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

            /**
             * 
             */
            void associate_screen(APP::IScreen* screenPtr) override final;

            /** \brief
             *  \param portName: 
             */
            int open_port(const char* portName) override final;

            /** \brief
             */
            int close_port() override final;

            /** \brief
             *  \param userData:
             */
            void catch_data(void* userData) override final;

            /** \brief
             *  \param userData:
             */
            void set_data(void* userData) override final;

            /** \brief
             *  \return
             */
            bool get_flag_serial() override final;

            /** \brief
             *  \return
             */
            APP::IdCapturer_t get_Id() override final;

        //====================
        // ATTRIBUTES
        //====================
        private:
            APP::IScreen*  screenPtr_;
            int            fd_ {-1};
            struct termios tty_;
            bool           state_;
    };
}
