/** \file devInspector.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date May 2026
 */
#pragma once

#include <libudev.h>
#include <glib-unix.h>
#include <string>
#include "application/IDevInspector.hpp"

namespace APP{
    struct IScreen;
}

namespace DRV_FRAMW{
    struct UsbDeviceInfo{
        std::string devNode;
        std::string manufacturer;
        std::string product;
        std::string serial;
        std::string vendorId;
        std::string productId;
    };

    /** \class Inspector.
     */
    class DevInspector: public APP::IDevInspector{
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
            DevInspector(const DevInspector &other) = delete;

            /** \note Move-Constructor disabled.
             *  \param other: InspectorDev object.
             */
            DevInspector(DevInspector &&other) = delete;

            /** \brief Destructor.
             */
            ~DevInspector();

            /** \note Assignment operator disabled.
             *  \param other: InspectorDev object.
             *  \return InspectorDev object.
             */
            DevInspector& operator =(const DevInspector &other) = delete;
            DevInspector& operator =(DevInspector &&other) = delete;

            /**
             * 
             */
            void associate_screen(APP::IScreen* screenPtr) override final;

            /** \brief Initializes the inspector of the devices.
             */
            void init() override final;

            void stop() override final;

            /** \brief Scan devices connected.
             */
            void scan_connected_devices();
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

            /** \brief Get the device info.
             *  \param ttyDev: Device.
             *  \return Device info.
             */
            static UsbDeviceInfo get_usb_device_info(struct udev_device* ttyDev);
        //====================
        // ATTRIBUTES
        //====================
        private:
            inline static APP::IScreen*        screenPtr_   {nullptr};
            struct udev*                       udevPt_      {nullptr};
            inline static struct udev_monitor* monitorPt_   {nullptr};
            guint                              idInspector_ {0};
    };
}
