/** \file vaoObject.hpp
 *  \author Edward Camilo
 *  \version v2.0
 *  \date June 2026
 */
#pragma once

#include <stdint.h>

/** \namespace DOMN.
 */
namespace DOMN{
   /** \enum VAO_COLOR_t.
    *  \class
    */
   enum class VAO_COLOR_t{
      RED,
      GREEN,
      BLUE,
      YELLOW,
      ORANGE,
      GRAY,
      BLACK,
      WHITE
   };

   /** \class VaoObject.
    */
   class VaoObject{
      //====================
      // METHODS
      //====================
      public:
         /** \note Principal-Constructor disabled.
          */
         VaoObject() = delete;

         /** \brief Constructor.
          *  \param numOfPoints: Number of points that make up the Object.
          *  \param indices: Number of indices that link the lines. 0 if is no necesary.
          *  \param color: Object color.
          */
         [[nodiscard]] explicit VaoObject(uint64_t numOfPoints, uint32_t indices, VAO_COLOR_t color);

         /** \note Copy-Constructor disabled.
          *  \param other: VaoObject to copy.
          */
         VaoObject(const VaoObject& other) = delete;

         /** \brief Move-Constructor.
          *  \param other: VaoObject to move.
          */
         VaoObject(VaoObject&& other) noexcept;

         /** \brief Destructor.
          */
         virtual ~VaoObject();

         /** \brief Assign operator.
          *  \param other: VaoObject object.
          *  \return VaoObject object.
          */
         VaoObject& operator =(const VaoObject& other) = delete;
         VaoObject& operator =(VaoObject&& other) noexcept;

         /** \brief Return the pointer to the vertices.
          *  \return Pointer to vertices.
          */
         float* get_vertices_pt() const;

         /** \brief Return the pointer to the color.
          *  \return Pointer to color.
          */
         float* get_color_pt() const;

         /** \brief Return the pointer to the elements.
          *  \return Pointer to elements;
          */
         unsigned int* get_elements_pt() const;

         /** \brief Returns the vertices length.
          *  \return Lenght (number of points).
          */
         uint64_t get_numOfPoints() const;

         /** \brief Returns the amount of indices.
          *  \return Number of indices.
          */
         uint64_t get_numOfInd() const;

         /** \brief Return the VAO reference;
          *  \return VAO reference.
          */
         uint32_t &get_vao();

         /** \brief Return the vertex indice reference;
          *  \return Vertex indice.
          */
         uint32_t &get_vertex_ind();

         /** \brief Return the color indice reference;
          *  \return Color indice.
          */
         uint32_t &get_color_ind();

         /** \brief Return the elements indice reference;
          *  \return Elements indice.
          */
         uint32_t &get_elements_ind();
      protected:
         /** \brief Assigns the color.
          *  \param color: VAO color.
          */
         virtual void assign_color(VAO_COLOR_t color);

         /** \brief Assigns the elements to build the object.
          */
         virtual void assign_elements();
      //====================
      // ATTRIBUTES
      //====================
      protected:
         float*        verticesPtr_ {nullptr};
         float*        colorPtr_    {nullptr};
         unsigned int* elementsPtr_ {nullptr};
      private:
         uint64_t      numOfPoints_ {0};
         uint64_t      numIndices_  {0};
         uint32_t      vao_         {0};
         uint32_t      vbo_[3]      {0, 0, 0};
   };
}
