/** \file ringBuffer.hpp
 *  \author Edward Camilo.
 *  \version v2.0
 *  \date June 2026
 */
#pragma once

#include <vector>

namespace ED{
    /** \struct
     */
    struct RingBuffer{
        //==========
        //~~~~~~~~~~
        // METHODS
        //~~~~~~~~~~
        //==========
        /** \note Default Constructor is disabled.
         */
        [[nodiscard]] RingBuffer() = delete;

        /** \brief Constructor.
         *  \param len: Length of the ring buffer.
         */
        [[nodiscard]] RingBuffer(size_t len);

        /** \note Copy-Constructor is disabled.
         *  \param other: Ring buffer to copy.
         */
        [[nodiscard]] RingBuffer(const RingBuffer &other) = delete;

        /** \note Move-Constructor is disabled.
         *  \param other: Ring buffer to pass.
         */
        [[nodiscard]] RingBuffer(RingBuffer &&other) = delete;

        /** \note Destructor.
         */
        ~RingBuffer();

        /** \note Assigment operator is disabled.
         *  \param other: Ring buffer to assign.
         *  \return The reference to the object ring. //eee
         */
        [[nodiscard]] RingBuffer& operator =(const RingBuffer &other) = delete;
        [[nodiscard]] RingBuffer& operator =(RingBuffer &&other) = delete;

        /** \brief Resets the ring buffer.
         */
        void reset();

        /** \brief Pass the data to the ring buffer.
         *  \param data: Pointer data to pass.
         *  \param w: Total data to pass.
         */
        void push_data(const float *data, size_t w);

        /** \brief Pass the data to the ring buffer.
         *  \param data: Pointer data to pass.
         *  \param w: Total data to pass.
         *  \param numBits: Number of bits by indice.
         */
        void push_data(const uint8_t* data, size_t w, uint8_t numBits, float voltRef);

        /** \brief Gets the array with the last n elements.
         *  \param bufStore: buffer where the data will be copy and stored.
         *  \param n: Number of elements to update in buffer to be delivered.
         *  \param jump: Number of data jumped for sample.
         */
        void get_n_data(float *bufStore, size_t n, unsigned int jump);

        /** \brief Shows the all the ring buffer.
         */
        void show_data() const;

        /** \brief Return the length.
         *  \return Ring buffer length.
         */
        size_t get_lenght();

        /** \brief Gets the pointer to some element at the buffer.
         *  \param i: Number of one element.
         *  \return Reference to the ring buffer.
         */
        float* get_ring_buffer_ref(unsigned int i) const;
        //==========
        //~~~~~~~~~~
        // ATTRIBUTES
        //~~~~~~~~~~
        //==========
        private:
            float  *ringBufferPt_ {nullptr};
            size_t indicator_     {0};
            size_t length_        {0};
    };
}
