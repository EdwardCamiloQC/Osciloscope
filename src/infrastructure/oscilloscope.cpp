#include <assert.h>
#include "infrastructure/oscilloscope.hpp"
#include "adapters/signalCapturer.hpp"
#include "frameworksAndDrivers/serialPort/serialPortPsoc.hpp"
#include "frameworksAndDrivers/deviceInspector/devInspector.hpp"
#include "frameworksAndDrivers/persistence/systemFile/docGenerator.hpp"
#include "adapters/screen.hpp"
#include "frameworksAndDrivers/gui/guiGTK.hpp"

using namespace INFRA;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
Oscilloscope::Oscilloscope():
    signalCapturerPtr_(std::make_unique<INFRA::SignalCapturer>(std::make_unique<DRV_FRAMW::SerialPortPsoc>())),
    devInspectorPtr_(std::make_unique<DRV_FRAMW::DevInspector>()),
    docGeneratorPtr_(std::make_unique<DRV_FRAMW::DocGenerator>()),
    screenPtr_(std::make_unique<INFRA::Screen>(std::make_unique<DRV_FRAMW::GuiGtk>(), 800, 500))
{
    assert(
        voltages_ != nullptr &&
        signalCapturerPtr_.get() != nullptr &&
        devInspectorPtr_.get() != nullptr &&
        docGeneratorPtr_.get() != nullptr &&
        screenPtr_.get() != nullptr
    );

    signalCapturerPtr_->associate_voltages(voltages_);
    signalCapturerPtr_->associate_screen(screenPtr_.get());

    devInspectorPtr_->associate_screen(screenPtr_.get());

    docGeneratorPtr_->associate_voltages(voltages_);

    screenPtr_->associate_signal_capturer(signalCapturerPtr_.get());
    screenPtr_->associate_dev_inspector(devInspectorPtr_.get());
    screenPtr_->associate_doc_generator(docGeneratorPtr_.get());
    screenPtr_->associate_voltages(voltages_);
}

int Oscilloscope::run(int& argc, char**& argv){
    if(screenPtr_)
        return screenPtr_->turn_on(argc, argv);

    return EXIT_FAILURE;
}
