/** \file puertoSerie.hpp
    \brief Declaración de la clase ComSerial que hace uso de la biblioteca SerialStream.h
    \author Edward Camilo
    \version 1.1 beta
    \date 2024
 */

#ifndef _PUERTO_SERIE_H_
    #define _PUERTO_SERIE_H_

    //extern "C"{
        #include <SerialStream.h>
    //}

    #include <sys/peripherals/capturer.hpp>

    class VoltageSignal;

    /**
        \class ComSerial
        \brief Clase que usa el puerto serial para la comunicación con el hardware externo 'Osciloscopio'
     */
    class ComSerial: public Capturer{
        /*Methods*/
        public:
            /**
                \brief Constructor que recibe las referencias de los voltajes que va a actualizar.
                \param volt1 puntero asociado al voltaje1
                \param volt2 puntero asociado al voltaje2
                \param volt3 puntero asociado al voltaje3
                \param volt4 puntero asociado al voltaje4
             */
            ComSerial(VoltageSignal *volt1, VoltageSignal *volt2, VoltageSignal *volt3, VoltageSignal *volt4);
            /**
                \brief Constructor de copia deshabilitado.
             */
            ComSerial(const ComSerial&) = delete;
            /**
                \brief Operador de asignación deshabilitado.
             */
            ComSerial& operator=(const ComSerial&) = delete;
            /**
                \brief Destructor. Cierra el puerto si llega a estar abierto.
             */
            ~ComSerial();
            /**
                \brief Abre un puerto serial.
                \param port : Ruta del archivo asociado al puertoserie generado por el kernel.
                \return Retorna true si el puerto está abierto o fasle si no lo está.
             */
            bool openPort(const char* port);
            /**
                \brief Cierra un puerto serial.
                \return Retorna true si cerró con normalidad o false si no.
             */
            bool closePort();
            /**
                \brief Retorna el estado del puertoserie.
                \return False si está cerrado, o true si está abierto.
             */
            bool getFlagSerial();
            /**
                \brief Lee 'n' valores del puerto serie para cada uno de los 4 voltajes asociados.
                Verificar que sean enviados corretamente desde el hardware externo 'Osciloscopio'
                \param nValues : cantidad de valores a leer por voltaje.
             */
            void readValues(unsigned int nValues) override;
            /**
                \brief Envía la velocidad de muestreo para la frecuencia requerida.
                \param freq : Frecuencia requerida.
             */
            void setSampleFrequency(unsigned int freq) override;
        private:
            /**
                \brief Desplaza las señales asociadas a los voltajes para su posterior captura
                \param n : Cantidad de valores a desplazar.
             */
            void scrollVoltages(unsigned int n);
            /**
                \brief Reviza si la línea capturada en el stream comienza por ciertos caracteres.
                \param line : String que obtiene la linea de los datos capturados por el stream.
                \param text : Caracteres de inicio.
             */
            bool startWith(std::string line, const char* text);

        /*Attributes*/
        private:
            LibSerial::SerialStream mySerial;  /**< SerialStream asociado a un puerto */
            VoltageSignal *voltage1 {nullptr}; /**< Voltaje1 */
            VoltageSignal *voltage2 {nullptr}; /**< Voltaje2 */
            VoltageSignal *voltage3 {nullptr}; /**< Voltaje3 */
            VoltageSignal *voltage4 {nullptr}; /**< Voltaje4 */
            float *valuesCatched1 {nullptr}; /**< Array asociado al señal voltaje1 */
            float *valuesCatched2 {nullptr}; /**< Array asociado al señal voltaje2 */
            float *valuesCatched3 {nullptr}; /**< Array asociado al señal voltaje3 */
            float *valuesCatched4 {nullptr}; /**< Array asociado al señal voltaje4 */
    };

#endif
