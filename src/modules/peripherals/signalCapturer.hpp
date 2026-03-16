/** \file signalCapturer.hpp
    \author Edward Camilo
    \version v1.0
    \date enero 2026
 */
#ifndef ED_SIGNAL_CAPTURER_H_
    #define ED_SIGNAL_CAPTURER_H_

    #include <thread>

    //==================================================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // FORWARD DECLARATION
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //==================================================
    class Capturer;
    class VoltageSignal;

    //==================================================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // CLASS
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //==================================================
    /** \brief It collects the data it that receives.
        \class
     */
    class SignalCapturer{
        //========== METHODS ==========
        public:
            /**
             */
            SignalCapturer();
            
            /** \brief Relates the capturer.
                \param theCapturer: Agregation with the capturer.
             */
            SignalCapturer(Capturer *capturer);

            /** \brief Destructor.
             */
            ~SignalCapturer();

            /** \brief Selects between the capturers.
                \param theCapturer: The capturer to use.
             */
            void select_capturer(std::unique_ptr<Capturer> &&capturer);

        //========== ATTRIBUTES ==========
        public:
            std::unique_ptr<Capturer> capturer_; ///< Instance to the especific capturer.
    };
#endif
