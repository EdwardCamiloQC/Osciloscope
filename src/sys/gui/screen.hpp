#ifndef _SCREEN_H_
    #define _SCREEN_H_

    #include <gtk/gtk.h>
    #include <epoxy/gl.h>
    #include <libraries/signal/voltageSignal.hpp>

    struct GridVoltage{
        unsigned int vao, vbo[2];
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
        };
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
        };
        void createGrid();
        void deleteGrid();
        void draw();
    };

    struct GridSpectrum{
        unsigned int vao, vbo[2];
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
        };
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
        };
        void createGrid();
        void deleteGrid();
        void draw();
    };

    class Screen{
        //  Methods
        public:
            Screen();
            ~Screen();
            Screen(const Screen& screen) = delete;
            Screen& operator=(Screen screen) = delete;
            int show(int &argc, char** argv);
            bool createVAO(SignalObject &signalObject);
            bool deleteVAO(SignalObject &signalObject);
            bool drawVAO(SignalObject &signalObject) const;

        //  Attributes
        private:
            GtkApplication *appGtk {nullptr};
        public:
            VoltageSignal voltage1;
            VoltageSignal voltage2;
            VoltageSignal voltage3;
            VoltageSignal voltage4;
            GtkWidget *window {nullptr};
            GtkWidget *boxPanels {nullptr};
            GtkWidget *boxView, *boxControl {nullptr};
            GtkWidget *glAreaVoltage, *glAreaSpectrum {nullptr};
            GtkWidget *buttonStartStop {nullptr};
            GtkWidget *separator1 {nullptr};
            GtkWidget *lavelVoltDiv {nullptr};
            GtkWidget *comboVoltDiv {nullptr};
            GtkWidget *gridSignals {nullptr};
            GtkWidget *checkSignal1 {nullptr};
            GtkWidget *checkSignal2 {nullptr};
            GtkWidget *checkSignal3 {nullptr};
            GtkWidget *checkSignal4 {nullptr};
            GtkAdjustment *adjustment1 {nullptr};
            GtkAdjustment *adjustment2 {nullptr};
            GtkAdjustment *adjustment3 {nullptr};
            GtkAdjustment *adjustment4 {nullptr};
            GtkWidget *spinOffset1 {nullptr};
            GtkWidget *spinOffset2 {nullptr};
            GtkWidget *spinOffset3 {nullptr};
            GtkWidget *spinOffset4 {nullptr};
            GtkWidget *separator2 {nullptr};
            GtkWidget *labelFreq {nullptr};
            GtkWidget *boxFreq {nullptr};
            GtkAdjustment *adjustmentFreq {nullptr};
            GtkWidget *spinFreq {nullptr};
            GtkWidget *comboFreq {nullptr};
            GtkWidget *separator3 {nullptr};
            GtkWidget *labelPort {nullptr};
            GtkWidget *boxPort {nullptr};
            GtkWidget *dropPort {nullptr};
            GtkWidget *buttonPort {nullptr};
            GridVoltage gridVoltage;
            GridSpectrum gridSpectrum;
            GLuint idShaderVolt {0}, idShaderSpec {0};
            bool stateStartStop_ {false};
            bool stateSignal1_{false}, stateSignal2_{false}, stateSignal3_{false}, stateSignal4_{false};
            float voltDiv_ {1.0f};
            float offset1_{0.0f}, offset2_{0.0f}, offset3_{0.0f}, offset4_{0.0f};
            unsigned int frequency_ {100}, multiplier_ {1};
    };
#endif
