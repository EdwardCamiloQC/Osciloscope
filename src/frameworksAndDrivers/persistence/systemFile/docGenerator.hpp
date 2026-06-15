/** \file docGenerator.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date June 2026
 */
#pragma once

#include "application/IDocGenerator.hpp"

/** \namespace DRV_FRAMW.
 */
namespace DRV_FRAMW{
    /** \class DocGenerator.
     */
    class DocGenerator: public APP::IDocGenerator{
        //====================
        // METHODS
        //====================
        public:
            /** \note Constructor by default.
             */
            DocGenerator() = default;

            /** \note Destructor by default.
             */
            ~DocGenerator() = default;

            /** \brief Reference the voltages.
             *  \param voltsPtr: Voltages.
             */
            void associate_voltages(DOMN::VoltageSignal* voltsPtr) override final;

            /** \brief Reference the screen.
             *  \param screenPtr: Screen.
             */
            void associate_screen(APP::IScreen* screenPtr) override final;

            /** \brief Reference the signal capturer.
             *  \param screenPtr: Signal Capturer.
             */
            void associate_signal_capturer(APP::ISignalCapturer* sigCapPtr) override final;

            /** \brief Saves the voltage signals.
             *  \param directory: Route path to the directory.
             *  \param type: Type of the file to save.
             */
            void generate_doc(const char *directory, APP::TYPE_DOC type, float timeDiv) const override final;
        //====================
        // ATTRIBUTES
        //====================
        private:
            DOMN::VoltageSignal*  voltsPtr_     {nullptr};
            APP::IScreen*         screenPtr_    {nullptr};
            APP::ISignalCapturer* signalCapPtr_ {nullptr};
    };
}
