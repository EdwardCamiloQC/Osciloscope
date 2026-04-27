/** \file inspectorDev.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date April 2026
 */
#pragma once

#include <libudev.h>
#include <glib-unix.h>

namespace DEV_INSPECTOR{
    /** \class Inspector.
     */
    class DevInspector{
        //====================
        // METHODS
        //====================
        public:
            /** \brief Constructor.
             */
            [[nodiscard]] DevInspector();

            /** \note Copy-Constructor disabled.
             *  \param other: InspectorDev object.
             */
            [[nodiscard]] DevInspector(const DevInspector &other) = delete;

            /** \note Move-Constructor disabled.
             *  \param other: InspectorDev object.
             */
            [[nodiscard]] DevInspector(DevInspector &&other) = delete;

            /** \brief Destructor.
             */
            ~DevInspector();

            /** \note Assignment operator disabled.
             *  \param other: InspectorDev object.
             *  \return InspectorDev object.
             */
            [[nodiscard]] DevInspector& operator =(const DevInspector &other) = delete;
            [[nodiscard]] DevInspector& operator =(DevInspector &&other) = delete;

            /** \brief Initializes the inspector of the devices.
             */
            void init();

            void stop();
        
        private:
            /** \brief Creates the contex to Udev.
                \return Status.
             */
            int create_context_udev();

            /** \brief Deletes the udev contex.
                \return Status.
             */
            int delete_context_udev();

            /** \brief Observes the devices conneted or disconnected.
                \param fd:
                \param condition:
                \param userData:
                \return
             */
            static gboolean udev_monitor_inspection(gint fd, GIOCondition condition, gpointer userData);

        //====================
        // ATTRIBUTES
        //====================
        private:
            struct udev                        *udevPt_     {nullptr};
            inline static struct udev_monitor  *monitorPt_  {nullptr};
            guint                              idInspector_ {0};
    };
}
