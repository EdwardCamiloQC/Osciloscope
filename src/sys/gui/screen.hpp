/** \file screen.hpp
    \brief The screen shows the signals and allows you to modify each one separately.
    \author Edward Camilo
    \version v1.0
    \date January 2026
 */
#ifndef _SCREEN_H_
    #define _SCREEN_H_

    #include <gtk/gtk.h>
    #include <epoxy/gl.h>

    //Forward declaration
    class SignalObject;

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

    /** \brief The screen display the signals and updates at a set time.
        \class
     */
    class Screen{
        // Attributes
        private:
            GtkApplication *appGtk {nullptr};
        public:
            GtkWidget *window {nullptr}; ///< Oscilloscope window.
            GtkWidget *boxPanels {nullptr}; ///< General boxes that contains the second boxes.
            GtkWidget *boxView {nullptr}; ///< Second box that contains print areas.
            GtkWidget *glAreaVoltage {nullptr}; ///< Print areas.
            GtkWidget *glAreaSpectrum {nullptr};
            GtkWidget *boxControl {nullptr}; ///< Second box that contains control area.
            GtkCssProvider *provider {nullptr}; ///< Style for start-stop button.
            GtkStyleContext *context {nullptr};
            GtkWidget *buttonStartStop {nullptr}; ///< Start stop.
            GtkWidget *separator1 {nullptr};
            GtkWidget *comboVoltDiv {nullptr}; ///< Options for volt/vid.
            GtkWidget *gridSignals {nullptr}; ///< .
            GtkWidget *checkSignal1 {nullptr}; ///< Show signal1 on-off.
            GtkWidget *checkSignal2 {nullptr}; ///< Show signal2 on-off.
            GtkWidget *checkSignal3 {nullptr}; ///< Show signal3 on-off.
            GtkWidget *checkSignal4 {nullptr}; ///< Show signal4 on-off.
            GtkAdjustment *adjustment1 {nullptr};
            GtkAdjustment *adjustment2 {nullptr};
            GtkAdjustment *adjustment3 {nullptr};
            GtkAdjustment *adjustment4 {nullptr};
            GtkWidget *spinOffset1 {nullptr}; ///< Change offset to signal1.
            GtkWidget *spinOffset2 {nullptr}; ///< Change offset to signal2.
            GtkWidget *spinOffset3 {nullptr}; ///< Change offset to signal3.
            GtkWidget *spinOffset4 {nullptr}; ///< Change offset to signal4.
            GtkWidget *separator2 {nullptr};
            GtkWidget *boxFreq {nullptr};
            GtkAdjustment *adjustmentFreq {nullptr};
            GtkWidget *spinFreq {nullptr}; ///< Change of the frequency.
            GtkWidget *comboFreq {nullptr}; ///< Range in frequency.
            GtkWidget *checkTestSignal {nullptr}; ///< Enable the test signal.
            GtkWidget *separator3 {nullptr};
            GtkWidget *labelPort {nullptr}; ///< Label that indicates the serial port.
            GtkWidget *boxPort {nullptr};
            GtkWidget *dropPort {nullptr}; ///< .
            GtkWidget *buttonPort {nullptr}; ///< Open or close the serial port.
            GridVoltage gridVoltage; ///< Grid voltage.
            GridSpectrum gridSpectrum; ///< Grid Spectrum.
            GLuint idShaderVolt {0}, idShaderSpec {0}; ///< Id shaders .
            bool stateStartStop_ {false}; ///< State that indicates when to update the signals or not.
            bool stateSignal1_{false}, stateSignal2_{false}, stateSignal3_{false}, stateSignal4_{false}; ///< State that indicates when the signals should or should not be displayed.
            float voltDiv_ {1.0f}; ///< current value needed to calculate the sampling rate.
            float offset1_{0.0f}, offset2_{0.0f}, offset3_{0.0f}, offset4_{0.0f}; ///< Voltage's offsets.

        // Methods
        public:
            /** \note Constructor.
             */
            Screen();

            /** \note Destructor.
             */
            ~Screen();

            /** \note Copy constructor disabled.
             */
            Screen(const Screen& screen) = delete;

            /** \note Operator assignment disabled.
             */
            Screen& operator=(Screen screen) = delete;

            /** \brief Creates the graphics and shows.
                \param argc: Argument count.
                \param argv: Argument vector.
                \return Error.
             */
            int show(int &argc, char** argv);

            /** \brief Creates the VAOs to the signals.
                \param signalObject: Array of the signal.
                \return Error.
             */
            bool createVAO(SignalObject &signalObject);

            /** \brief Deletes the signal's VAOs.
                \param signalObject: Array of the signal.
                \return Error.
             */
            bool deleteVAO(SignalObject &signalObject);

            /** \brief Draws the signal.
                \param signalObject: Array of the signal.
                \return Error.
             */
            bool drawVAO(SignalObject &signalObject) const;
    };
#endif
