/** \file vaoObject.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date April 2026
 */
#pragma once

#include <entities/signals/signalObject.hpp>

namespace VAOS_OBJECTS{
   /** \class VaoObject.
    */
   class VaoObject{
      public:
         /** \brief Creates the VAOs to the signals.
          *  \param signalObject: Array of the signal.
          *  \return Error.
          */
         static bool createVAO(SignalObject &signalObject);

         /** \brief Deletes the signal's VAOs.
          *  \param signalObject: Array of the signal.
          *  \return Error.
          */
         static bool deleteVAO(SignalObject &signalObject);

         /** \brief Draws the signal.
          *  \param signalObject: Array of the signal.
          *  \return Error.
          */
         static bool drawVAO(SignalObject &signalObject);
   };
}
