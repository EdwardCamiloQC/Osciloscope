/** \file guiGTK.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date April 2026
 */
#pragma once

#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include <atomic>

#include <adapters/gui.hpp>
#include <entities/grids/gridVoltage.hpp>
#include <entities/grids/gridSpectrum.hpp>

namespace GUI_GTK{
    /** \class
     */
    class GuiGtk : public Gui{
        //====================
        // METHODS
        //====================
        public:
            /** \brief Constructor.
             */
            [[nodiscard]] GuiGtk();

            /** \note Copy-constructor disabled.
             *  \param other: GuiGtk object.
             */
            [[nodiscard]] GuiGtk(const GuiGtk &other) = delete;

            /** \note Move-constructor disabled.
             *  \param other: GuiGtk object.
             */
            [[nodiscard]] GuiGtk(GuiGtk &other) = delete;

            /** \brief Destructor.
             */
            ~GuiGtk();

            /** \note Assigment operator disabled.
             *  \param other: GuiGtk object.
             *  \return Non return.
             */
            [[nodiscard]] GuiGtk& operator =(const GuiGtk &other) = delete;
            [[nodiscard]] GuiGtk& operator =(GuiGtk &&other) = delete;

            /** \brief
             *  \param argc:
             *  \param argv:
             *  \return Status.
             */
            int init(int &argc, char **&argv) override;

            /** \brief Updates the drop down list.
                \param add:
                \param text:
             */
            void update_drop_port(const bool add, const char *text) override;

        private:
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
            inline static GtkApplication      *appGtkPtr_            {nullptr};
            inline static GtkWidget           *glAreaVoltagePtr_     {nullptr};
            inline static GtkWidget           *glAreaSpectrumPtr_    {nullptr};
            inline static GtkWidget           *dropPortPtr_          {nullptr};
            inline static GtkWidget           *buttonPortPtr_        {nullptr};
            inline static GtkTextBuffer       *bufferConsolePtr_     {nullptr};
            inline static GtkStringList       *deviceListPortPtr_    {nullptr};
            inline static GridVoltage         gridVoltage_;
            inline static GridSpectrum        gridSpectrum_;
            inline static GLuint              idShaderVolt_          {0};
            inline static GLuint              idShaderSpec_          {0};
            inline static bool                stateStartStop_        {false};
            inline static bool                stateSignal1_          {false};
            inline static bool                stateSignal2_          {false};
            inline static bool                stateSignal3_          {false};
            inline static bool                stateSignal4_          {false};
            inline static double              voltDiv_               {1.0f};
            inline static float               offset1_               {0.0f};
            inline static float               offset2_               {0.0f};
            inline static float               offset3_               {0.0f};
            inline static float               offset4_               {0.0f};
            inline static guint               drawTimeOutID_         {0};
            inline static std::string         routePort_;

        public:
            inline static std::atomic<double> timeDiv_ {1.0f};
    };
}
