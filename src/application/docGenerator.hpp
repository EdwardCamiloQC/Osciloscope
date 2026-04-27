/** \file docGenerator.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date April 2026
 */
#pragma once

#include <string>

namespace DOC_GENERATOR{
    /** \enum TYPE_DOC
     *  \class
     */
    enum class TYPE_DOC{
        CSV,
        TSV,
        TXT,
        JSON
    };

    /** \class DocGenerator.
     */
    class DocGenerator{
        //====================
        // METHODS
        //====================
        public:
            /** \brief Saves the voltage signals.
             *  \param directory: Route path to the directory.
             *  \param type: Type of the file to save.
             */
            void save_doc(const char *directory, TYPE_DOC type) const;
    };
}
