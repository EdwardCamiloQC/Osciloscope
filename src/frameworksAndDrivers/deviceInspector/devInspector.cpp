#include "frameworksAndDrivers/deviceInspector/devInspector.hpp"
#include "application/IScreen.hpp"

using namespace DRV_FRAMW;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
DevInspector::DevInspector(){
    create_context_udev();
}

DevInspector::~DevInspector(){
    delete_context_udev();
}

void DevInspector::associate_screen(APP::IScreen* screenPtr){
    screenPtr_ = screenPtr;
}

void DevInspector::init(){
    scan_connected_devices();

    int udevFd = udev_monitor_get_fd(monitorPt_);
    if(udevFd >= 0){
        idInspector_ = g_unix_fd_add(udevFd, G_IO_IN, DevInspector::udev_monitor_inspection, this);
    }
}

void DevInspector::stop(){
    if(idInspector_){
        g_source_remove(idInspector_);
        idInspector_ = 0;
    }
}

void DevInspector::scan_connected_devices(){
    if(!udevPt_ || !screenPtr_)
        return;

    struct udev_enumerate* enumeratePtr = udev_enumerate_new(udevPt_);
    if(!enumeratePtr)
        return;

    udev_enumerate_add_match_subsystem(enumeratePtr, "tty");
    udev_enumerate_scan_devices(enumeratePtr);

    struct udev_list_entry* devicesPtr = udev_enumerate_get_list_entry(enumeratePtr);

    struct udev_list_entry* entryPtr;

    udev_list_entry_foreach(entryPtr, devicesPtr){
        const char* path = udev_list_entry_get_name(entryPtr);

        struct udev_device* devPtr = udev_device_new_from_syspath(udevPt_, path);
        if(!devPtr)
            continue;

        UsbDeviceInfo info = get_usb_device_info(devPtr);

        if(info.product == "OsciloscopioExternalDevice")
        {
            screenPtr_->add_device(info.devNode.c_str());
        }

        udev_device_unref(devPtr);
    }

    udev_enumerate_unref(enumeratePtr);
}
//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
int DevInspector::create_context_udev(){
    //crear contexto udev
	udevPt_ = udev_new();
	if(!udevPt_){
        if(screenPtr_)
		    screenPtr_->set_message("No se pudo crear el contexto udev\n", 2);
		return EXIT_FAILURE;
	}

	//Crear monitor para recibir eventos desde udev a través de netlink
	monitorPt_ = udev_monitor_new_from_netlink(udevPt_, "udev");
	if(!monitorPt_){
		screenPtr_->set_message("No se pudo crear el monitor de udev\n", 2);
		udev_unref(udevPt_);
		return EXIT_FAILURE;
	}

	//Filtrar eventos para el subsistema "tty"
	if(udev_monitor_filter_add_match_subsystem_devtype(monitorPt_, "tty", nullptr) < 0){
		screenPtr_->set_message("Error al aplicar el filtro para el subsistema tty\n", 2);
		udev_monitor_unref(monitorPt_);
		udev_unref(udevPt_);
		return EXIT_FAILURE;
	}

	//Habilitar la recepcion de eventos
	if(udev_monitor_enable_receiving(monitorPt_) < 0){
		screenPtr_->set_message("Error al habilitar la recepcion de eventos\n", 2);
		udev_monitor_unref(monitorPt_);
		udev_unref(udevPt_);
		return EXIT_FAILURE;
	}

    return EXIT_SUCCESS;
}

int DevInspector::delete_context_udev(){
    if(monitorPt_){
        udev_monitor_unref(monitorPt_);
    }
    if(udevPt_){
        udev_unref(udevPt_);
    }

    return EXIT_SUCCESS;
}

gboolean DevInspector::udev_monitor_inspection([[maybe_unused]]gint fd, [[maybe_unused]]GIOCondition condition, [[maybe_unused]]gpointer userData){
    if(!monitorPt_)
        return FALSE;

    struct udev_device *dev = udev_monitor_receive_device(monitorPt_);
    if(!dev){
        return TRUE;
    }

    const char *action = udev_device_get_action(dev);
    UsbDeviceInfo info = get_usb_device_info(dev);

    if(action){
        if(strcmp(action, "add") == 0){
            if(info.product == "OsciloscopioExternalDevice"){
                if(screenPtr_)
                    screenPtr_->add_device(info.devNode.c_str());
            }
        }else if(strcmp(action, "remove") == 0){
            if(screenPtr_)
                screenPtr_->remove_device(info.devNode.c_str());
        }
    }

    udev_device_unref(dev);

    return TRUE;
}

UsbDeviceInfo DevInspector::get_usb_device_info(struct udev_device* ttyDev){
    UsbDeviceInfo info;

    if(!ttyDev)
        return info;

    const char* node = udev_device_get_devnode(ttyDev);
    if(node)
        info.devNode = node;

    struct udev_device* usb = udev_device_get_parent_with_subsystem_devtype(ttyDev, "usb", "usb_device");
    if(!usb)
        return info;

    auto readProperty = [&](const char* name)->std::string{
        const char* value = udev_device_get_sysattr_value(usb, name);
        return value ? value : "";
    };

    info.manufacturer = readProperty("manufacturer");
    info.product      = readProperty("product");
    info.serial       = readProperty("serial");
    info.vendorId     = readProperty("idVendor");
    info.productId    = readProperty("idProduct");
    return info;
}
