/** \file docGenerator.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date May 2026
 */
#pragma once

#include <string>
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
            /**
             * 
             */
            DocGenerator() = default;

            /**
             * 
             */
            ~DocGenerator() = default;

            /**
             * 
             */
            void associate_voltages(DOMN::VoltageSignal* voltsPtr) override final;

            /** \brief Saves the voltage signals.
             *  \param directory: Route path to the directory.
             *  \param type: Type of the file to save.
             */
            void generate_doc(const char *directory, APP::TYPE_DOC type, float timeDiv) const override final;
    };
}
