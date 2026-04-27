/** \file ringBuffer.hpp
 *  \author Edward Camilo.
 *  \version v1.0
 *  \date April 2026
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

        /** \brief Gets the array with the last n elements.
         *  \param bufStore: buffer where the data will be copy and stored.
         *  \param n: Number of elements to update in buffer to be delivered.
         */
        void get_n_data(float *bufStore, size_t n);

        /** \brief Shows the all the ring buffer.
         */
        void show_data() const;

        //==========
        //~~~~~~~~~~
        // ATTRIBUTES
        //~~~~~~~~~~
        //==========
        private:
            float  *ringBufferPt_ {nullptr};
            float  *endPt_        {nullptr};
            float  *indicatorPt_   {nullptr};
            size_t length_        {0};
    };
}
