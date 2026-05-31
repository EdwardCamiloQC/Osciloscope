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
    /** \brief It collects the data that receive its.
        \class SignalCapturer.
     */
    class SignalCapturer: public APP::ISignalCapturer{
        //====================
        // METHODS
        //====================
        public:
            /** \note Principal Constructor disabled.
             */
            SignalCapturer() = delete;

            /** \brief Relates the capturer.
                \param theCapturer: Agregation with the capturer selected.
             */
            [[nodiscard]] explicit SignalCapturer(std::unique_ptr<APP::ICapturer> capturer);

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
            void select_capturer(std::unique_ptr<APP::ICapturer> &&capturer);

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
            void associate_screen(APP::IScreen* screenPtr);

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

            /**
             * 
             */
            void open_close_port(const char* portName) override final;

            DOMN::VoltageSignal* get_voltages_ref();
        private:
            /** \brief Catch the voltages.
             */
            void catch_loop();

            /**
             * 
             */
            void add_ns(timespec& t, long ns);
        //====================
        // ATTRIBUTES
        //====================
        private:
            APP::IScreen*                   screenPtr_ {nullptr};
            DOMN::VoltageSignal*            voltages_ {nullptr};
            std::unique_ptr<APP::ICapturer> capturerPtr_; ///< Instance to the especific capturer.
            std::thread                     catcher_;
        public:
            std::atomic<bool>               stateCatcher_;
            std::mutex                      mutexCatcher_;
    };
}
