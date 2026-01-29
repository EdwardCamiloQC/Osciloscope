#include <iostream>
#include <chrono>

#include <sys/peripherals/signalCapturer.hpp>
#include <sys/oscilloscope.hpp>
#include <sys/peripherals/capturer.hpp>

//~~~~~~~~~~
//      PUBLIC METHODS
//~~~~~~~~~~
SignalCapturer::SignalCapturer(Capturer *theCapturer)
    :capturer(theCapturer){
}

SignalCapturer::~SignalCapturer(){
    if(catcher.joinable()){
        catcher.join();
    }
}

void SignalCapturer::start(unsigned int nValues){
    catcher = std::thread(&SignalCapturer::loopCatchVoltages, this, nValues);
}

void SignalCapturer::setSampleFrequency(unsigned int freq){
    frequency = freq;
}

void SignalCapturer::setMultiplierFrequency(unsigned int mult){
    multiplier = mult;
}

void SignalCapturer::selectCapturer(std::unique_ptr<Capturer> &&theCapturer){
    capturer = std::move(theCapturer);
}

//~~~~~~~~~~
//      PRIVATE METHODS
//~~~~~~~~~~
void SignalCapturer::loopCatchVoltages(unsigned int nValues){
    Oscilloscope *osc = Oscilloscope::getInstance();
    while(osc->stateOnOff_){
        if(capturer){
            //shift the voltages
            osc->voltage1.shiftVoltage(nValues);
            osc->voltage2.shiftVoltage(nValues);
            osc->voltage3.shiftVoltage(nValues);
            osc->voltage4.shiftVoltage(nValues);
            capturer->readValues(&(osc->voltage1),
                                &(osc->voltage2),
                                &(osc->voltage3),
                                &(osc->voltage4),
                                nValues);
            osc->voltage1.calculateSpectrum();
            osc->voltage2.calculateSpectrum();
            osc->voltage3.calculateSpectrum();
            osc->voltage4.calculateSpectrum();
        }else{
            std::cerr << "without concrete capturer" << std::endl;
        }
        //std::this_thread::sleep_for(std::chrono::seconds(2));
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<unsigned int>(1000/(frequency*multiplier))));
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
