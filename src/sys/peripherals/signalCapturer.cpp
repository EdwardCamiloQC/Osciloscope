#include <iostream>
#include <chrono>

#include <sys/peripherals/signalCapturer.hpp>
#include <sys/oscilloscope.hpp>
#include <sys/peripherals/capturer.hpp>

//~~~~~~~~~~
//      PUBLIC METHODS
//~~~~~~~~~~
SignalCapturer::SignalCapturer(Screen* theScreen, Capturer *theCapturer)
    :capturer(theCapturer), screen(theScreen){
}

SignalCapturer::~SignalCapturer(){
    if(catcher.joinable()){
        catcher.join();
    }
}

void SignalCapturer::start(){
    catcher = std::thread(&SignalCapturer::loop, this);
}

void SignalCapturer::selectCapturer(std::unique_ptr<Capturer> &&theCapturer){
    capturer = std::move(theCapturer);
}

//~~~~~~~~~~
//      PRIVATE METHODS
//~~~~~~~~~~
void SignalCapturer::loop(){
    Oscilloscope *osc = Oscilloscope::getInstance();
    while(osc->stateOnOff_){
        catchVoltages();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void SignalCapturer::catchVoltages(){
    if(capturer){
        capturer->readValues(1);
    }
}

/*void SignalCapturer::inspection(){
    Oscilloscope* ptGUI = (Oscilloscope*)userData;

    //Obtener el descritor de archivos del monitor para usarlo con select
	int fd = udev_monitor_get_fd(ptGUI->monitor);
	std::cout << "Monitoreando eventos en el descriptor:" << fd << std::endl;

    while(ptGUI->flagInspection){
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		//Bloquea hasta que haya un evento
		int ret = select(fd + 1, &fds, nullptr, nullptr, nullptr);
		if(ret > 0 && FD_ISSET(fd, &fds)){
			//Recibir el dispositivo que genero el evento
			struct udev_device *dev = udev_monitor_receive_device(ptGUI->monitor);
			if(dev){
				//Obtener la accion: add para la conexion, remove para la desconexion, etc
				const char *action = udev_device_get_action(dev);
				//Obtener el modo del dispositivo (por ejemplo, /dev/ttyUSB0)
				const char *devnode = udev_device_get_devnode(dev);
				if(action && devnode){
					if(strcmp(action, "add") == 0){
                        ComboBoxTextUpdate *addData = (ComboBoxTextUpdate*)g_malloc(sizeof(ComboBoxTextUpdate));
                        addData->combo = GTK_COMBO_BOX_TEXT((ptGUI->windowOsc).control.comboBoxPort);
                        addData->text = g_strdup(devnode);
                        addData->add = TRUE;
                        g_idle_add(updateComboBoxPort, addData);
                    }else if(strcmp(action, "remove") == 0){
                        ComboBoxTextUpdate *removeData = (ComboBoxTextUpdate*)g_malloc(sizeof(ComboBoxTextUpdate));
                        removeData->combo = GTK_COMBO_BOX_TEXT((ptGUI->windowOsc).control.comboBoxPort);
                        removeData->text = g_strdup(devnode);
                        removeData->add = FALSE;
                        g_idle_add(updateComboBoxPort, removeData);
                    }
				}
				udev_device_unref(dev);
			}
		}else if(ret < 0){
			std::cerr << "Error en select()" << std::endl;
			break;
		}
	}
    //g_idle_add(funcion, userData);
}*/
