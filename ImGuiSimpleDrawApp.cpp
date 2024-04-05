#include "ImGuiSimpleDrawApp.h"
#include "imgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <ctime>
#ifndef IM_PI
#define IM_PI 3.14159265358979323846f
#endif
namespace ImGuiSimpleDrawApp
{
    static ImGuiTextBuffer log_com;
    unsigned char buffer_serial[1024];
    int initial_msg = 50;
    int initial_msg_labels = 12;
    int initial_radius = 300;
    int initial_angle = 0;
    int second_angle = 360;
    bool redraw_circle = true;
    int current_hour = 0;
    int current_minute = 0;
    int current_second = 0;
    ImVec4 selected_color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Inițializează culoarea selectată la roșu
    ImVec4 contur_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Inițializează culoarea selectată la roșu
    ImVec4 clock_pointers_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Inițializează culoarea selectată la roșu

    ImVec4 clock_axes_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Inițializează culoarea selectată la gray

    void
    draw_current_time()
    {
        // Obține timpul curent
        std::time_t current_time = std::time(nullptr);

        // Converteste timpul curent la structura localtime
        std::tm *timeinfo = std::localtime(&current_time);

        // Extrage orele și minutele
        current_hour = timeinfo->tm_hour;
        current_minute = timeinfo->tm_min;
        current_second = timeinfo->tm_sec;

        // Construieste un string pentru a afisa ora curenta
        char time_str[9];
        std::strftime(time_str, sizeof(time_str), "%T", timeinfo);

        // Afiseaza ora curenta in fereastra ImGui
        ImGui::Text("Local time: %s", time_str);
    }

    void draw_circle(ImDrawList *draw_list, ImVec2 origin, float radius, ImU32 color, int num_segments = 12, float thickness = 0.5f)
    {
        const float segment_angle = 2.0f * IM_PI / num_segments;
        for (int i = 0; i < num_segments; i++)
        {
            float a0 = segment_angle * i;
            float a1 = segment_angle * (i + 1);
            draw_list->AddLine(ImVec2(origin.x + cosf(a0) * radius, origin.y + sinf(a0) * radius),
                               ImVec2(origin.x + cosf(a1) * radius, origin.y + sinf(a1) * radius),
                               color, thickness);
        }
    }

    void draw_line_with_angle(ImDrawList *draw_list, ImVec2 origin, float length, float angle, ImU32 color, float thickness = 0.5f)
    {
        ImVec2 end;
        end.x = origin.x + cosf(angle) * length;
        end.y = origin.y + sinf(angle) * length;
        draw_list->AddLine(origin, end, color, thickness);
    }
    void draw_circle_with_axes(ImDrawList *draw_list, ImVec2 origin, float radius, ImU32 color, int num_segments = 12, float thickness = 0.5f)
    {
        // Desenare cerc
        draw_circle(draw_list, origin, radius, color, num_segments,thickness);

        // Desenare axele cercului
        ImVec2 center = ImVec2(origin.x , origin.y );
        // Coordonatele centrului cercului
        draw_list->AddLine(ImVec2(center.x - radius, center.y), ImVec2(center.x + radius, center.y), color, 0.5f); // Axă orizontală
        draw_list->AddLine(ImVec2(center.x, center.y - radius), ImVec2(center.x, center.y + radius), color, 0.5f); // Axă verticală
    }
    void draw_circle_with_axes_and_labels(ImDrawList *draw_list, ImVec2 origin, float radius, ImU32 color, int num_labels = 12, float thickness = 0.5f)
    {
        // Desenare cerc și axele cercului
        draw_circle_with_axes(draw_list, origin, radius, color, thickness);

        // Coordonatele centrului cercului
        ImVec2 center = ImVec2(origin.x + radius, origin.y + radius);

        // Desenare cifre în jurul cercului
        for (int i = 0; i < num_labels; ++i)
        {
            float angle = 2.0f * IM_PI * i / num_labels;
            ImVec2 label_pos;
            label_pos.x = center.x + cosf(angle) * (radius + 10); // Amplasare etichetă la o distanță de raza cercului + 10
            label_pos.y = center.y + sinf(angle) * (radius + 10);
            char label[3];                                                           // String pentru a stoca cifra și terminatorul nul
            sprintf(label, "%d", i + 1);                                             // Convertește numărul la șir de caractere
            draw_list->AddText(label_pos, ImGui::GetColorU32(ImGuiCol_Text), label); // Adaugă text la poziția calculată
        }
    }
    void draw_circle_with_axes_and_separate_labels(ImDrawList *draw_list, ImVec2 origin, float radius, ImU32 color, int num_labels = 12, float thickness = 0.5f)
    {

        // Setează fontul cu o scară mai mare
        ImGuiIO &io = ImGui::GetIO();
        float old_font_scale = io.FontGlobalScale;
        io.FontGlobalScale = 1.5f;


        // Desenare cerc și axele cercului
        draw_circle_with_axes(draw_list, origin, radius, color, thickness);

        // Coordonatele centrului cercului
        ImVec2 center = ImVec2(origin.x, origin.y);

        // Calcularea unghiului dintre fiecare etichetă
        float angle_step = 2.0f * IM_PI / num_labels;

        // Desenare cifre în jurul cercului
        for (int i = 0; i < num_labels; ++i)
        {
            float angle = (angle_step * i) - (angle_step *2.0f); //+ (IM_PI / 2.0f); // Rotirea unghiului pentru a plasa cifra 12 în partea de sus
            ImVec2 label_pos;
            label_pos.x = center.x + cosf(angle) * (radius - 20); // Amplasare etichetă la o distanță de raza cercului + 10
            label_pos.y = center.y + sinf(angle) * (radius - 20);
            char label[3];                                                           // String pentru a stoca cifra și terminatorul nul
            sprintf(label, "%d", (12 + i + 1) % 12 == 0 ? 12 : (12 + i + 1) % 12);   // Convertește numărul la șir de caractere
            //draw_list->AddText(label_pos, ImGui::GetColorU32(ImGuiCol_Text), label); // Adaugă text la poziția calculată
            draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize() * io.FontGlobalScale, label_pos, ImGui::GetColorU32(ImGuiCol_Text), label);
        }
        // Desenare axe diagonale
        for (int i = 0; i < num_labels; ++i)
        {
            float angle = angle_step * i;
            ImVec2 diag_endpoint;
            diag_endpoint.x = center.x + cosf(angle) * radius;
            diag_endpoint.y = center.y + sinf(angle) * radius;
            draw_list->AddLine(center, diag_endpoint, color, 0.5f); // Adaugă linie diagonală de la centru la punctul de pe cerc
        }
    }
    void draw_filled_circle_segment(ImDrawList *draw_list, ImVec2 origin, ImVec2 center, float radius, float thickness, float start_angle, float end_angle, ImU32 color)
    {
        int num_segments = 30; // Numărul de segmente pentru a aproxima cercul
        float angle_step = (end_angle - start_angle) / num_segments;

        // Desenează conturul segmentului de cerc
        for (int i = 0; i < num_segments; ++i)
        {
            float angle0 = start_angle + i * angle_step;
            float angle1 = start_angle + (i + 1) * angle_step;
            draw_list->AddLine(ImVec2(origin.x + cosf(angle0) * radius, origin.y + sinf(angle0) * radius),
                               ImVec2(origin.x + cosf(angle1) * radius, origin.y + sinf(angle1) * radius),
                               color, thickness);
        }

        // Desenează segmentul de cerc umplut
        ImVec2 p_center(origin.x, origin.y);
        for (int i = 0; i < num_segments; ++i)
        {
            float angle0 = start_angle + i * angle_step;
            float angle1 = start_angle + (i + 1) * angle_step;
            draw_list->AddTriangleFilled(p_center,
                                         ImVec2(origin.x + cosf(angle0) * radius, origin.y + sinf(angle0) * radius),
                                         ImVec2(origin.x + cosf(angle1) * radius, origin.y + sinf(angle1) * radius),
                                         color);
        }
    }

    int draw_circle_test()
    {
        // Desenare cerc împărțit în 12 felii
        ImVec2 center = ImGui::GetCursorScreenPos();
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        float thickness = 2.5f;
        float start_angle = (initial_angle * IM_PI) / 180.0f;
        float end_angle = initial_angle + (second_angle * IM_PI) / 180.0f; // Exemplu: segment de la 0 la 90 de grade
        ImU32 segment_color = IM_COL32(selected_color.x * 255, selected_color.y * 255, selected_color.z * 255, selected_color.w * 255);
        // Desenează un segment de cerc umplut
        draw_filled_circle_segment(draw_list, ImVec2(center.x + initial_radius, center.y + initial_radius), center, initial_radius, thickness, start_angle, end_angle, segment_color);
        ImU32 clock_contur_color = IM_COL32(contur_color.x * 255, contur_color.y * 255, contur_color.z * 255, contur_color.w * 255);
        draw_circle_with_axes(draw_list, ImVec2(center.x + initial_radius, center.y + initial_radius), initial_radius, clock_contur_color, initial_msg, 6.0f);
        ImU32 clk_axes_color = IM_COL32(clock_axes_color.x * 255, clock_axes_color.y * 255, clock_axes_color.z * 255, clock_axes_color.w * 255);
        draw_circle_with_axes_and_separate_labels(draw_list, ImVec2(center.x + initial_radius, center.y + initial_radius), initial_radius, clk_axes_color, initial_msg_labels, 0.5f);

        ImU32 clk_pointers_color = IM_COL32(clock_pointers_color.x * 255, clock_pointers_color.y * 255, clock_pointers_color.z * 255, clock_pointers_color.w * 255);
        // Desenare linie cu unghi dat - minutar
        draw_line_with_angle(draw_list, ImVec2(center.x + initial_radius, center.y + initial_radius), initial_radius - 5, (float)(current_minute - 15) * IM_PI / 30.0, clk_pointers_color, 7.0f);
        // Desenare linie cu unghi dat - limba orei
        draw_line_with_angle(draw_list, ImVec2(center.x + initial_radius, center.y + initial_radius), initial_radius - 20, (float)(current_hour - 15) * (2.0 * IM_PI / 12.0), clk_pointers_color, 7.0f);
        // Desenare linie cu unghi dat - secundar
        draw_line_with_angle(draw_list, ImVec2(center.x + initial_radius, center.y + initial_radius), initial_radius + 2, (float)(current_second - 15) * (IM_PI / 30.0), clk_pointers_color, 3.0f);
        

        // Afiseaza culoarea selectată
        //ImGui::Text("Culoarea selectata: (%.2f, %.2f, %.2f, %.2f)", selected_color.x, selected_color.y, selected_color.z, selected_color.w);

        draw_current_time();
       
        return 0;
    }

   

    void RenderUi(void)
    {

        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", nullptr, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();

                if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode;
                }
                if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit;
                }
                if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking;
                }
                if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
                }
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
                }
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
                }
                ImGui::Separator();

                // if (ImGui::MenuItem("Close", NULL, false))
                //     *p_open = false;
                ImGui::EndMenu();
            }


            ImGui::EndMenuBar();
        }
        ImGui::Begin("Settings");
        ImGui::InputInt("Input number segments", &initial_msg);
        ImGui::InputInt("Input radius", &initial_radius);
        ImGui::InputInt("Input angle", &initial_angle);
        ImGui::InputInt("Input second angle", &second_angle);
        // Afișează color picker-ul și permite utilizatorului să selecteze o culoare
        //ImGui::ColorPicker4("Selectează culoarea", (float *)&selected_color);
        // Afișează un buton pentru deschiderea color picker-ului în modul popup
        if (ImGui::Button("Open color picker background clock"))
        {
            ImGui::OpenPopup("MyColorPickerPopup1"); // Deschide popup-ul color picker-ului
        }

        // Verifică dacă popup-ul color picker-ului este deschis
        if (ImGui::BeginPopup("MyColorPickerPopup1"))
        {
            // Afișează color picker-ul și permite utilizatorului să selecteze o culoare
            ImGui::ColorPicker4("Select the color", (float *)&selected_color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueBar);

            ImGui::EndPopup(); // Încheie popup-ul color picker-ului
        }
        // Afișează un buton pentru deschiderea color picker-ului în modul popup
        if (ImGui::Button("Open color picker clock contour"))
        {
            ImGui::OpenPopup("MyColorPickerPopup2"); // Deschide popup-ul color picker-ului
        }

        // Verifică dacă popup-ul color picker-ului este deschis
        if (ImGui::BeginPopup("MyColorPickerPopup2"))
        {
            // Afișează color picker-ul și permite utilizatorului să selecteze o culoare
            ImGui::ColorPicker4("Select the color", (float *)&contur_color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueBar);

            ImGui::EndPopup(); // Încheie popup-ul color picker-ului
        }
        // Afișează un buton pentru deschiderea color picker-ului în modul popup
        if (ImGui::Button("Open color picker clock pointers"))
        {
            ImGui::OpenPopup("MyColorPickerPopup3"); // Deschide popup-ul color picker-ului
        }

        // Verifică dacă popup-ul color picker-ului este deschis
        if (ImGui::BeginPopup("MyColorPickerPopup3"))
        {
            // Afișează color picker-ul și permite utilizatorului să selecteze o culoare
            ImGui::ColorPicker4("Select the color", (float *)&clock_pointers_color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueBar);

            ImGui::EndPopup(); // Încheie popup-ul color picker-ului
        }

        // Afișează un buton pentru deschiderea color picker-ului în modul popup
        if (ImGui::Button("Open color picker axes"))
        {
            ImGui::OpenPopup("MyColorPickerPopup4"); // Deschide popup-ul color picker-ului
        }

        // Verifică dacă popup-ul color picker-ului este deschis
        if (ImGui::BeginPopup("MyColorPickerPopup4"))
        {
            // Afișează color picker-ul și permite utilizatorului să selecteze o culoare
            ImGui::ColorPicker4("Select the color", (float *)&clock_axes_color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueBar);

            ImGui::EndPopup(); // Încheie popup-ul color picker-ului
        }

        ImGui::End(); // Settings

        ImGui::Begin("Viewport");
        ImGuiSimpleDrawApp::draw_circle_test();
        ImGui::End(); // Viewport

        ImGui::End(); //"DockSpace Demo
    }
}