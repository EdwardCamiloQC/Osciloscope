/** \file gui.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date April 2026
 */
#pragma once

/** \struct Gui (Graphic User Interface)
 */
struct Gui{
    //====================
    // METHODS
    //====================
    /** \note Virtual Destructor.
     */
    virtual ~Gui() = default;

    /** \brief Inits the graphic user interface.
     *  \param argc: Argument count.
     *  \param argv: Argument vector.
     *  \return Status.
     */
    virtual int init(int &argc, char **&argv) = 0;

    /** \brief
     *  \param
     *  \param portDevice:
     */
    virtual void update_drop_port(bool add, const char *portDevice) = 0;
};
