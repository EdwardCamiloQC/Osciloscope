/** \file signalCapturer.hpp
    \author Edward Camilo
    \version v1.1
    \date April 2026
 */
#pragma once

#include <thread>
#include <atomic>

struct Capturer;
class VoltageSignal;

namespace SIGNAL_CAPTURER{
    /** \brief It collects the data that receive its.
        \class SignalCapturer.
     */
    class SignalCapturer{
        //====================
        // METHODS
        //====================
        public:
            /** \note Principal Constructor disabled.
             */
            [[nodiscard]] SignalCapturer() = delete;
            
            /** \brief Relates the capturer.
                \param theCapturer: Agregation with the capturer selected.
             */
            [[nodiscard]] SignalCapturer(std::unique_ptr<Capturer> capturer);

            /** \note Copy-Constructor disabled.
             *  \param other: SignalCapturer object.
             */
            [[nodiscard]] SignalCapturer(const SignalCapturer &other) = delete;

            /** \note Move-Constructor disabled.
             *  \param other: SignalCapturer object.
             */
            [[nodiscard]] SignalCapturer(SignalCapturer &&other) = delete;

            /** \note Destructor.
             */
            ~SignalCapturer();

            /** \note Assignment operator disabled.
             *  \param other: SignalCapturer object.
             *  \return SignalCapturer object.
             */
            [[nodiscard]] SignalCapturer& operator =(const SignalCapturer &other) = delete;
            [[nodiscard]] SignalCapturer& operator =(SignalCapturer &&other) = delete;

            /** \brief Selects between the capturers.
                \param theCapturer: The capturer to use.
             */
            void select_capturer(std::unique_ptr<Capturer> &&capturer);

            /** \brief Runs the thread.
             *  \param userData: Pointer to user data.
             */
            void init(void *userData);

        private:
            /** \brief Catch the voltages.
             *  \param volt1: Pointer to the VoltageSignal 1.
             *  \param volt2: Pointer to the VoltageSignal 2.
             *  \param volt3: Pointer to the VoltageSignal 3.
             *  \param volt4: Pointer to the VoltageSignal 4.
             *  \param m: Number of values to catching.
             *  \param userData:
             */
            void catch_voltages(void *userData);

        //====================
        // ATTRIBUTES
        //====================
        public:
            std::unique_ptr<Capturer> capturer_; ///< Instance to the especific capturer.
        
        private:
            std::thread        catcher_;
    };
}
