/** \file guiGTK.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date April 2026
 */
#pragma once

#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include <string>
#include "application/IGui.hpp"
#include "frameworksAndDrivers/gui/grids/gridVoltage.hpp"
#include "frameworksAndDrivers/gui/grids/gridSpectrum.hpp"

namespace APP{
    struct ISignalCapturer;
    struct IDevInspector;
    struct IDocGenerator;
}

/** \namespace DRV_FRAMW.
 */
namespace DRV_FRAMW{
    /** \class GuiGtk.
     */
    class GuiGtk : public APP::IGui{
        //====================
        // METHODS
        //====================
        public:
            /** \brief Returns the instance.
             *  \return GuiGtk object.
             */
            static GuiGtk& get_instance();

            /** \brief Associates one signal capturer to the GUI.
             *  \param sigCapPtr: Pointer reference to SignalCapturer.
             */
            void associate_signal_capturer(APP::ISignalCapturer* sigCapPtr) override final;

            /** \brief Assocciates one device inspector.
             *  \param devInspPtr: Reference to one device inspector.
             */
            void associate_dev_inspector(APP::IDevInspector* devInspPtr) override final;

            /** \brief Associates once doc generator.
             *  \param docGenPtr: Reference to one doc generator.
             */
            void associate_doc_generator(APP::IDocGenerator* docGenPtr) override final;

            /** \brief Associates the voltages.
             *  \param voltsPtr: Reference to the Signal voltages.
             */
            void associate_voltages(DOMN::VoltageSignal* voltsPtr) override final;

            /** \brief Initializes the GUI and also the SignalCapturer and DeviceInspector.
             *  \param argc: Argument count.
             *  \param argv: Argument vector.
             *  \return Status.
             */
            int init(int &argc, char **&argv) override final;

            /** \brief Updates the drop down list.
                \param add: Flag to represent add or remove.
                \param text: Name of the port.
             */
            void update_drop_port(const bool add, const char *text) override final;

            /** \brief Show in GUI the message that arrive.
             *  \param msg: Message.
             *  \param type: 0->in 1->normal 2->error 3->warning 4->open 5->close.
             */
            void display_message(const char* msg, int type) const override final;

            /** \brief Changes the label in button that correspond to the serial port.
             *  \param open: Flag to represent if the serial port was openned or closed.
             */
            void update_label_button_port(bool open) override final;
        private:
            /** \brief Constructor.
             */
            [[nodiscard]] GuiGtk();

            /** \note Copy-constructor disabled.
             *  \param other: GuiGtk object.
             */
            GuiGtk(const GuiGtk &other) = delete;

            /** \note Move-constructor disabled.
             *  \param other: GuiGtk object.
             */
            GuiGtk(GuiGtk &other) = delete;

            /** \brief Destructor.
             */
            ~GuiGtk();

            /** \note Assigment operator disabled.
             *  \param other: GuiGtk object.
             *  \return Non return.
             */
            GuiGtk& operator =(const GuiGtk &other) = delete;
            GuiGtk& operator =(GuiGtk &&other) = delete;

            /** \brief Show in GUI the message send proper of this class.
             *  \param msg: Message.
             *  \param type: 0->in 1->normal 2->error 3->warning 4->open 5->close.
             */
            static void display_message_static(const char* msg, int type);

            /** \brief Constructs the window app.
                \param app:
                \param userData:
             */
            static void construct_window_callback(GtkApplication* app, gpointer userData);

            /** \brief Destroy the window app.
                \param widget:
                \param userData:
             */
            static void destroy_window_callback(GtkWidget *widget, gpointer userData);

            /** \brief Realize the graphic OpenGL (voltage grig and signals).
                \param area:
                \param userData:
             */
            static void realize_voltage_callback(GtkGLArea *area, gpointer userData);

            /** \brief Unrealize the graphic OpenGL (voltage grid and signals).
                \param area:
                \param userData:
             */
            static void unrealize_voltage_callback(GtkGLArea *area, gpointer userData);

            /** \brief Render the graphic OpenGL (voltage grid and signals).
                \param area:
                \param context:
                \param userData:
                \return The state for if renderes again or no.
             */
            static gboolean render_voltage_callback(GtkGLArea *area, GdkGLContext *context, gpointer userData);

            /** \brief Resize the graphic OpenGL (voltage grid and signals).
                \param area:
                \param userData:
             */
            static void resize_voltage_callback(GtkGLArea *area, gpointer userData);

            /** \brief Realize the graphic OpenGL (spectrum grig and signals).
                \param area:
                \param userData:
             */
            static void realize_spectrum_callback(GtkGLArea *area, gpointer userData);

            /** \brief Unrealize the graphic OpenGL (spectrum grid and signals).
                \param area:
                \param userData:
             */
            static void unrealize_spectrum_callback(GtkGLArea *area, gpointer userData);

            /** \brief Render the graphic OpenGL (spectrum grid and signals).
                \param area:
                \param context:
                \param userData:
                \return The state for if renderes again or no.
             */
            static gboolean render_spectrum_callback(GtkGLArea *area, GdkGLContext *context, gpointer userData);

            /** \brief Resize the graphic OpenGL (voltage grid and signals).
                \param area:
                \param userData:
             */
            static void resize_spectrum_callback();

            /** \brief Updates the flag that indicates the state start stop.
                \param widget:
                \param userData:
             */
            static void startStop_callback(GtkWidget *widget, gpointer userData);

            /** \brief Updates the voltDiv value.
                \param widget:
                \param userData:
             */
            static void voltDiv_callback(GtkDropDown *widget, gpointer userData);

            /** \brief Updates the flag that indicates if the signal1 is enable or disabled to display.
                \param widget:
                \param nSignal:
             */
            static void check_signal_callback(GtkCheckButton *widget, gpointer nSignal);

            /** \brief Updates the offset value of the singal1.
                \param spinButton:
                \param nSiganl:
             */
            static void offset_signal_callback(GtkSpinButton *spinButton, gpointer nSignal);

            /** \brief Updates the timeDiv value.
                \param spinButton:
                \param userData:
             */
            static void spin_button_freq_callback(GtkSpinButton *spinButton, gpointer userData);

            /** \brief
             *  \param button:
             *  \param userData:
             */
            static void button_doc_callback(GtkButton *button, gpointer userData);

            /** \brief Update the drop down listing the connect-disconnect devices and selects the routePort.
                \param widget: The drop down.
                \param userData: Pointer to use.
             */
            static void drop_port_callback(GtkDropDown *widget, gpointer userData);

            /** \brief Opens or closes the port serial.
                \param widget:
                \param userData:
             */
            static void button_port_callback(GtkWidget *widget, gpointer userData);

            /** \brief
                \param controller:
                \param x:
                \param y:
                \param userData:
             */
            static void click_callback(GtkGestureClick *gesture, int nPress, double x, double y, gpointer userData);

            /** \brief Draw the graphics indicates to GTK.
                \param userData:
                \return
             */
            static gboolean render(gpointer userData);
        //====================
        // ATTRIBUTES
        //====================
        private:
            inline static GtkApplication*       appGtkPtr_            {nullptr};
            inline static GtkWidget*            glAreaVoltagePtr_     {nullptr};
            inline static GtkWidget*            glAreaSpectrumPtr_    {nullptr};
            inline static GtkWidget*            dropPortPtr_          {nullptr};
            inline static GtkWidget*            buttonPortPtr_        {nullptr};
            inline static GtkTextBuffer*        bufferConsolePtr_     {nullptr};
            inline static GtkStringList*        deviceListPortPtr_    {nullptr};
            inline static APP::ISignalCapturer* signalCapturerPtr_    {nullptr};
            inline static APP::IDevInspector*   devInspectorPtr_      {nullptr};
            inline static APP::IDocGenerator*   docGeneratorPtr_      {nullptr};
            inline static DOMN::VoltageSignal*  voltagesPtr_          {nullptr};
            inline static GridVoltage           gridVoltage_;
            inline static GridSpectrum          gridSpectrum_;
            inline static GLuint                idShaderVolt_         {0};
            inline static GLuint                idShaderSpec_         {0};
            inline static bool                  stateStartStop_       {false};
            inline static bool                  stateSignal_[4]       {false, false, false, false};
            inline static double                voltDiv_              {1.0f};
            inline static float                 offsets_[4]           {0.0f, 0.0f, 0.0f, 0.0f};
            inline static guint                 drawTimeOutID_        {0};
            inline static std::string           routePort_;
        public:
            inline static double                timeDiv_              {1.0f};
    };
}
