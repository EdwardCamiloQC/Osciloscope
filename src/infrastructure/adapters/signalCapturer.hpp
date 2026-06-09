/** \file signalCapturer.hpp
 *  \author Edward Camilo
 *  \version v1.1
 *  \date May 2026
 */
#pragma once

#include <thread>
#include <atomic>
#include <time.h>
#include "application/ISignalCapturer.hpp"

namespace APP{
    struct ICapturer;
}
namespace DOMN{
    class VoltageSignal;
}

/** \namespace INFRA.
 */
namespace INFRA{
    enum class Capturer_t{
        PSOC,
        ANY_MCU
    };

    /** \brief It collects the data that receive its.
        \class SignalCapturer.
     */
    class SignalCapturer: public APP::ISignalCapturer{
        //====================
        // METHODS
        //====================
        public:
            /** \brief Principal-Constructor.
             */
            [[nodiscard]] SignalCapturer();

            /** \note Copy-Constructor disabled.
             *  \param other: SignalCapturer object.
             */
            SignalCapturer(const SignalCapturer &other) = delete;

            /** \note Move-Constructor disabled.
             *  \param other: SignalCapturer object.
             */
            SignalCapturer(SignalCapturer &&other) = delete;

            /** \note Destructor.
             */
            ~SignalCapturer();

            /** \note Assignment operator disabled.
             *  \param other: SignalCapturer object.
             *  \return SignalCapturer object.
             */
            SignalCapturer& operator =(const SignalCapturer &other) = delete;
            SignalCapturer& operator =(SignalCapturer &&other) = delete;

            /** \brief Selects between the capturers.
                \param theCapturer: The capturer to use.
             */
            void select_capturer(Capturer_t capturer);

            /** \brief Runs the thread.
             *  \param userData: Pointer to user data.
             */
            void start_reading() override final;

            /** \brief Stops the thread.
             */
            void stop_reading() override final;

            /**
             * 
             */
            void associate_screen(APP::IScreen* screenPtr) override final;

            /** \brief
             *  \param voltsPtr:
             */
            void associate_voltages(DOMN::VoltageSignal* voltsPtr) override final;

            /** \brief
             *  \param
             */
            void set_time_div(double timeDiv) override final;

            /**
             * 
             */
            std::mutex& get_mutex() override final;

            /** \brief Opens the serial port if it was already closed. Or, closes it if it was already open.
             *  \param portName: Serial Port name.
             */
            void open_close_port(const char* portName) override final;

            /** \brief Gets the pointer to the voltages.
             *  \return VoltageSignal pointer.
             */
            DOMN::VoltageSignal* get_voltages_ref();
        private:
            /** \brief Catch the voltages.
             */
            void catch_loop();

            /** \brief Calculates the time difference.
             *  \note Applies the difference in t1.
             *  \param t1: First time.
             *  \param t2: Second time.
             */
            void time_difference(timespec& t1, timespec& t2);
        //====================
        // ATTRIBUTES
        //====================
        private:
            APP::IScreen*         screenPtr_ {nullptr};
            DOMN::VoltageSignal*  voltages_ {nullptr};
            APP::ICapturer&       capturer_;
            std::thread           catcher_;
        public:
            std::atomic<bool>     stateCatcher_;
            std::mutex            mutexCatcher_;
    };
}
