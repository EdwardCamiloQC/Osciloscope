/** \file oscilloscope.hpp
    \brief The Oscilloscope was designed to display signals that arrive from the serial port.
    \author Edward Camilo
    \version v3.0
    \date February 2026
 */
#ifndef ED_OSCILLOSCOPE_H_
    #define ED_OSCILLOSCOPE_H_

    #include <mutex>
    #include <gtk/gtk.h>
    #include <epoxy/gl.h>
    #include <libudev.h>

    #include <modules/peripherals/signalCapturer.hpp>
    #include <utils/signal/voltageSignal.hpp>

    //==================================================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // FORWARD DECLARATION
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //==================================================
    class SignalObject;

    //==================================================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TYPES DECLARATIONS
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //==================================================
    /** \brief The structure that contains the grid shape where the voltage signals will be printed.
        \struct GridVoltage
     */
     struct GridVoltage{
        unsigned int vao, vbo[2]; ///< Vertex Array Object and Vertex Buffers Objects.
        float vertex[56]={
            //x1   y1     x2     y2
            -0.25f, +1.0f, -0.25f, -1.0f,
            -0.50f, +1.0f, -0.50f, -1.0f,
            -0.75f, +1.0f, -0.75f, -1.0f,
            +0.00f, +1.0f, +0.00f, -1.0f,
            +0.25f, +1.0f, +0.25f, -1.0f,
            +0.50f, +1.0f, +0.50f, -1.0f,
            +0.75f, +1.0f, +0.75f, -1.0f,
            //x1   y1    x2     y2
            -1.0f, -0.25f, +1.0f, -0.25f,
            -1.0f, -0.50f, +1.0f, -0.50f,
            -1.0f, -0.75f, +1.0f, -0.75f,
            -1.0f, +0.00f, +1.0f, +0.00f,
            -1.0f, +0.25f, +1.0f, +0.25f,
            -1.0f, +0.50f, +1.0f, +0.50f,
            -1.0f, +0.75f, +1.0f, +0.75f
        }; ///< Vertices belonging to the signals grid.
        float color[84]={
            //R     G       B       R       G       B
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            //R     G       B       R       G       B
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f
        }; ///< Grid color.

        /** \brief Creates the signals grid.
         */
        void createGrid();

        /** \brief Deletes the signals grid.
         */
        void deleteGrid();

        /** \brief Draws the signals grid*/
        void draw();
     };

    /** \brief The structure that contains the grid shape where the spectrum signals will be printed.
        \struct GridSpectrum.
     */
     struct GridSpectrum{
        unsigned int vao, vbo[2]; ///< Vertex Array Object and Vertex Buffer Objects.
        static constexpr float vertex[32]={
            //x1    y1      x2      y2
            -0.25f, +1.0f, -0.25f, -1.0f,
            -0.50f, +1.0f, -0.50f, -1.0f,
            -0.75f, +1.0f, -0.75f, -1.0f,
            +0.00f, +1.0f, +0.00f, -1.0f,
            +0.25f, +1.0f, +0.25f, -1.0f,
            +0.50f, +1.0f, +0.50f, -1.0f,
            +0.75f, +1.0f, +0.75f, -1.0f,
            //x1    y1      x2      y2
            -1.0f, -1.0f,   1.0f,  -1.0f
        }; ///< Vertices belonging to the spectrum grid.
        static constexpr float color[48]={
            //R     G       B       R       G       B
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
            //R     G       B       R       G       B
            0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f
        }; ///< Spectrum grid color.

        /** \brief Creates the spectrum grid.
         */
        void createGrid();

        /** \brief Deletes the spectrum grid.
         */
        void deleteGrid();

        /** \brief Draws the spectrum grid.
         */
        void draw();
     };

    //==================================================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // CLASS
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //==================================================
    /** \brief The Oscilloscope class displays four signals that arrive from the serial port.
            So there, are only one instance of this class.
        \class Oscilloscope.
     */
     class Oscilloscope{
        public:
            /** \note Copy-constructor disabled.
             */
             Oscilloscope(const Oscilloscope& oscilloscope) = delete;

            /** \note Move-constructor disabled.
             */
             Oscilloscope(Oscilloscope &&oscilloscope) = delete;

            /** \note Assignment operator disabled.
             */
             Oscilloscope& operator =(const Oscilloscope &oscilloscope) = delete;
             Oscilloscope& operator =(Oscilloscope &&oscilloscope) = delete;

            /** \brief Operator bool.
             */
             [[nodiscard]] operator bool()const;

            /** \brief Gets the instance.
                \return Oscilloscope*: Returns the already created instance.
             */
             static Oscilloscope* get_instance();

            /** \brief Destroys the instance.
             */
             static void destroy_instance();

            /** \brief Runs the program.
                \param argc: Gets the argument count.
                \param argv: Gets the argument vector.
                \return Returns the any error with a int value.
             */
             int run(int& argc, char**& argv);

        private:
            /** \brief Constructor creates the instance.
                \note Is private.
             */
             [[nodiscard]] Oscilloscope();

            /** \brief Destructor deletes the instance.
                \note Is private.
             */
             ~Oscilloscope();

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
             static void check_signal1_callback(GtkCheckButton *widget, gpointer nSignal);

            /** \brief Updates the flag that indicates if the signal2 is enable or disabled to display.
                \param widget:
                \param nSignal:
             */
             static void check_signal2_callback(GtkCheckButton *widget, gpointer nSignal);

            /** \brief Updates the flag that indicates if the signal3 is enable or disabled to display.
                \param widget:
                \param nSignal:
             */
             static void check_signal3_callback(GtkCheckButton *widget, gpointer nSignal);

            /** \brief Updates the flag that indicates if the signal4 is enable or disabled to display.
                \param widget:
                \param nSignal:
             */
             static void check_signal4_callback(GtkCheckButton *widget, gpointer nSignal);

            /** \brief Updates the offset value of the singal1.
                \param spinButton:
                \param nSiganl:
             */
             static void offset_signal1_callback(GtkSpinButton *spinButton, gpointer nSignal);

            /** \brief Updates the offset value of the singal2.
                \param spinButton:
                \param nSiganl:
             */
             static void offset_signal2_callback(GtkSpinButton *spinButton, gpointer nSignal);

            /** \brief Updates the offset value of the singal3.
                \param spinButton:
                \param nSiganl:
             */
             static void offset_signal3_callback(GtkSpinButton *spinButton, gpointer nSignal);

            /** \brief Updates the offset value of the singal4.
                \param spinButton:
                \param nSiganl:
             */
             static void offset_signal4_callback(GtkSpinButton *spinButton, gpointer nSignal);

            /** \brief Updates the timeDiv value.
                \param spinButton:
                \param userData:
             */
             static void spin_button_freq_callback(GtkSpinButton *spinButton, gpointer userData);

            /** \brief
                \param widget:
                \param userData:
             */
             static void check_test_signal_callback(GtkCheckButton *widget, gpointer userData);

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
            
            /** \brief Observes the devices conneted or disconnected.
                \param fd:
                \param condition:
                \param userData:
                \return
             */
             static gboolean udev_monitor_inspection(gint fd, GIOCondition condition, gpointer userData);

            /** \brief Catch the voltages.
                \param userData:
                \return
             */
             static gboolean catch_voltages(gpointer userData);

            /** \brief Draw the graphics indicates to GTK.
                \param userData:
                \return
             */
             static gboolean draw_signals(gpointer userData);
            
            /** \brief Creates the contex to Udev.
                \return Status.
             */
             int createContextUdev();

            /** \brief Deletes the udev contex.
                \return Status.
             */
             int deleteContextUdev();

            /** \brief Creates the VAOs to the signals.
                \param signalObject: Array of the signal.
                \return Error.
             */
             static bool createVAO(SignalObject &signalObject);

            /** \brief Deletes the signal's VAOs.
                \param signalObject: Array of the signal.
                \return Error.
             */
             static bool deleteVAO(SignalObject &signalObject);

            /** \brief Draws the signal.
                \param signalObject: Array of the signal.
                \return Error.
             */
             static bool drawVAO(SignalObject &signalObject);

            /** \brief Updates the drop down list.
                \param add:
                \param text:
             */
             static void updateDropPort(const bool add, const char *text); 
        
        private:
            static constexpr unsigned long LENGTH_VOLT_SIGNAL {128};
            static constexpr unsigned int  M_VALUES_TO_SHIFT  {4};
            static Oscilloscope        *instance_pt_;
            static GtkApplication      *appGtk_pt_;
            static GtkWidget           *glAreaVoltage_pt_;
            static GtkWidget           *glAreaSpectrum_pt_;
            static GtkWidget           *dropPort_pt_;
            static GtkWidget           *buttonPort_pt_;
            static GtkTextBuffer       *bufferConsole_pt_;
            static GtkStringList       *deviceListPort_pt_;
            static struct udev         *udev_pt_;
            static struct udev_monitor *monitor_pt_;
            static SignalCapturer      signalCapturer_;
            static VoltageSignal       voltage1_;
            static VoltageSignal       voltage2_;
            static VoltageSignal       voltage3_;
            static VoltageSignal       voltage4_;
            static std::string         routePort_;
            static GridVoltage         gridVoltage_;
            static GridSpectrum        gridSpectrum_;
            static GLuint              idShaderVolt_;
            static GLuint              idShaderSpec_;
            static bool                stateStartStop_;
            static bool                stateSignal1_;
            static bool                stateSignal2_;
            static bool                stateSignal3_;
            static bool                stateSignal4_;
            static float               voltDiv_;
            static float               offset1_;
            static float               offset2_;
            static float               offset3_;
            static float               offset4_;
            static double              timeDiv_;
            static guint               udevSourceID_;
            static guint               drawTimeOutID_;
            static unsigned int        shiftNvalues_;
        public:
            static std::mutex              mutex_; ///<
     };
#endif
