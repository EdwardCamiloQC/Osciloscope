#include <iostream>

#include <application/inspectorDev.hpp>
#include <application/oscilloscope.hpp>

using namespace DEV_INSPECTOR;

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

void DevInspector::init(){
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

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
int DevInspector::create_context_udev(){
    //crear contexto udev
	udevPt_ = udev_new();
	if(!udevPt_){
		std::cerr << "No se pudo crear el contexto udev\n";
		return EXIT_FAILURE;
	}

	//Crear monitor para recibir eventos desde udev a través de netlink
	monitorPt_ = udev_monitor_new_from_netlink(udevPt_, "udev");
	if(!monitorPt_){
		std::cerr << "No se pudo crear el monitor de udev\n";
		udev_unref(udevPt_);
		return EXIT_FAILURE;
	}

	//Filtrar eventos para el subsistema "tty"
	if(udev_monitor_filter_add_match_subsystem_devtype(monitorPt_, "tty", nullptr) < 0){
		std::cerr << "Error al aplicar el filtro para el subsistema tty\n";
		udev_monitor_unref(monitorPt_);
		udev_unref(udevPt_);
		return EXIT_FAILURE;
	}

	//Habilitar la recepcion de eventos
	if(udev_monitor_enable_receiving(monitorPt_) < 0){
		std::cerr << "Error al habilitar la recepcion de eventos\n";
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
    
    auto *osc = OSCILLOSCOPE::Oscilloscope::get_instance();

    struct udev_device *dev = udev_monitor_receive_device(monitorPt_);
    if(!dev){
        return TRUE;
    }

    const char *action = udev_device_get_action(dev);
    const char *devnode = udev_device_get_devnode(dev);

    if(action && devnode){
        if(strcmp(action, "add") == 0){
            gchar *copy = g_strdup(devnode);
            osc->screen_.gui_->update_drop_port(true, copy);
            g_free(copy);
        }else if(strcmp(action, "remove") == 0){
            gchar *copy = g_strdup(devnode);
            osc->screen_.gui_->update_drop_port(false, copy);
            g_free(copy);
        }
    }

    udev_device_unref(dev);

    return TRUE;
}
