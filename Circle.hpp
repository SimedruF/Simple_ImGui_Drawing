#include <iostream>
#include <math.h>
#include <cmath>
#include <ctime>
#ifndef IM_PI
#define IM_PI 3.14159265358979323846f
#endif
class Circle
{
private:
    float radius;
    float thickness;
    std::pair<float, float> origin; // Pair of floats for x and y coordinates of origin
    float start_angle;
    int numbers_radius; // Number of divisions for radius
    int numbers_pos;    // Number of positions for labels
    int current_hour;
    int current_minute;
    int current_second;
    bool show_text;
    int index;
    float rotation_speed;
    int blade_index;
    float current_vibration;

public:
    // Constructor
    Circle(float r, float t, std::pair<float, float> o, float sa, int nr, int np) : radius(r), thickness(t), origin(o), start_angle(sa), numbers_radius(nr), numbers_pos(np)
    {
        index = 0;
    }

    // Getters
    float getRadius() const { return radius; }
    float getThickness() const { return thickness; }
    std::pair<float, float> getOrigin() const { return origin; }
    float getStartAngle() const { return start_angle; }
    int getNumbersRadius() const { return numbers_radius; }
    int getNumbersPos() const { return numbers_pos; }
    int getCurrentHour() { return current_hour; }
    int getCurrentMin() { return current_minute; }
    int getCurrentSec() { return current_second; }
    int getShowText() { return show_text; }

    float getRotSpeed() { return rotation_speed; }
    int getBladeIndex() { return blade_index; }
    int getCurrentVibration() { return current_vibration; }

    // Setters
    void setRadius(float r) { radius = r; }
    void setThickness(float t) { thickness = t; }
    void setOrigin(std::pair<float, float> o) { origin = o; }
    void setStartAngle(float sa) { start_angle = sa; }
    void setNumbersRadius(int nr) { numbers_radius = nr; }
    void setNumbersPos(int np) { numbers_pos = np; }
    void setCurrentHour(float ct) {  current_hour= ct; }
    void setCurrentMin(int cm) { current_minute = cm; }
    void setCurrentSec(int cs) { current_second = cs; }
    void setShowText(bool st) { show_text = st; }
    void setIndex(int indx) { index = indx; }
    void setRotSpeed(int rs) { rotation_speed=rs; }
    void setBladeIndex(int bi) { blade_index=bi; }
    void setCurrentVibration(float cv) { current_vibration = cv; }

    // Method to display circle information
    void displayInfo() const
    {
        std::cout << "Circle Information:" << std::endl;
        std::cout << "Radius: " << radius << std::endl;
        std::cout << "Thickness: " << thickness << std::endl;
        std::cout << "Origin: (" << origin.first << ", " << origin.second << ")" << std::endl;
        std::cout << "Start Angle: " << start_angle << std::endl;
        std::cout << "Numbers Radius: " << numbers_radius << std::endl;
        std::cout << "Numbers Position: " << numbers_pos << std::endl;
        std::cout << "Blade index: " << blade_index<< std::endl;
        std::cout << "Rotation speed: " << rotation_speed << std::endl;
        std::cout << "Current vibration: " << current_vibration << std::endl;

    }
    void draw_current_time()
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
        if (show_text)
        {
            // Afiseaza ora curenta in fereastra ImGui
            ImGui::Text("Local time: %s", time_str);
        }
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
        draw_circle(draw_list, origin, radius, color, num_segments, thickness);

        // Desenare axele cercului
        ImVec2 center = ImVec2(origin.x, origin.y);
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
            label_pos.x = center.x + (cosf(angle) * numbers_radius) + 10; // Amplasare etichetă la o distanță de raza cercului + 10
            label_pos.y = center.y + (sinf(angle) * numbers_radius) + 10;
            char label[3];                                                           // String pentru a stoca cifra și terminatorul nul
            sprintf(label, "%d", i + 1);                                             // Convertește numărul la șir de caractere
            draw_list->AddText(label_pos, ImGui::GetColorU32(ImGuiCol_Text), label); // Adaugă text la poziția calculată
        }
    }
    void draw_circle_with_axes_and_separate_labels(ImDrawList *draw_list, ImVec2 origin, float radius, ImU32 color, int num_labels = 12, float thickness = 0.5f,float font_size=1.0f)
    {

        // Setează fontul cu o scară mai mare
        ImGuiIO &io = ImGui::GetIO();
        float old_font_scale = io.FontGlobalScale;
        io.FontGlobalScale = font_size;

        // Desenare cerc și axele cercului
        draw_circle_with_axes(draw_list, origin, radius, color, thickness);

        // Coordonatele centrului cercului
        ImVec2 center = ImVec2(origin.x, origin.y);

        // Calcularea unghiului dintre fiecare etichetă
        float angle_step = 2.0f * IM_PI / num_labels;

        // Desenare cifre în jurul cercului
        for (int i = 0; i < num_labels; ++i)
        {
            float angle = (angle_step * i) - (angle_step * 2.0f);  // Rotirea unghiului pentru a plasa cifra 12 în partea de sus
            ImVec2 label_pos;
            label_pos.x = center.x + (cosf(angle) * numbers_radius) - numbers_pos; // Amplasare etichetă la o distanță de raza cercului
            label_pos.y = center.y + (sinf(angle) * numbers_radius) - numbers_pos;
            char label[3];                                                         // String pentru a stoca cifra și terminatorul nul
            sprintf(label, "%d", (12 + i + 1) % 12 == 0 ? 12 : (12 + i + 1) % 12); // Convertește numărul la șir de caractere
            // draw_list->AddText(label_pos, ImGui::GetColorU32(ImGuiCol_Text), label); // Adaugă text la poziția calculată
            draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize() * font_size, label_pos, ImGui::GetColorU32(ImGuiCol_Text), label);
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
    void draw_circle_with_axes_and_labels_with_font(ImDrawList *draw_list, ImVec2 origin, float radius, ImU32 color, int num_labels = 12, float thickness = 0.5f, float font_size = 1.0f)
    {

        // Setează fontul cu o scară mai mare
        ImGuiIO &io = ImGui::GetIO();
        float old_font_scale = io.FontGlobalScale;
        io.FontGlobalScale = font_size;

        // Desenare cerc și axele cercului
        draw_circle_with_axes(draw_list, origin, radius, color, thickness);

        // Coordonatele centrului cercului
        ImVec2 center = ImVec2(origin.x, origin.y);

        // Calcularea unghiului dintre fiecare etichetă
        float angle_step = 2.0f * IM_PI / num_labels;

        // Desenare cifre în jurul cercului
        for (int i = 0; i < num_labels; ++i)
        {
            float angle = (angle_step * i) - (angle_step * 2.0f);  // Rotirea unghiului pentru a plasa cifra 12 în partea de sus
            ImVec2 label_pos;
            label_pos.x = center.x + (cosf(angle) * numbers_radius) - numbers_pos; // Amplasare etichetă la o distanță de raza cercului
            label_pos.y = center.y + (sinf(angle) * numbers_radius) - numbers_pos;
            char label[3];                                                         // String pentru a stoca cifra și terminatorul nul
            sprintf(label, "%d",i+1); // Convertește numărul la șir de caractere
            draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize() * font_size, label_pos, ImGui::GetColorU32(ImGuiCol_Text), label);
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
    void draw_filled_circle_segment(ImDrawList *draw_list, ImVec2 origin, float radius, float thickness, float start_angle, float end_angle, ImU32 color)
    {
        int num_segments = 30; // Numărul de segmente pentru a aproxima cercul
        float angle_step = (end_angle - start_angle) / num_segments;

        // Desenează conturul segmentului de cerc
        for (int i = 0; i < num_segments; ++i)
        {
            float angle0 = start_angle + i * angle_step;
            float angle1 = start_angle + (i + 1) * angle_step;
            // Calculează culorea cu transparență în funcție de unghi
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
    void draw_colored_circle_segment(ImDrawList *draw_list, ImVec2 center, float radius, float thickness, int segments, int colored_segment_index, ImU32 color)
    {
        float angle_step = (2.0f * IM_PI) / segments;

        for (int i = 0; i < segments; ++i)
        {
            float angle0 = i * angle_step - (angle_step * 2.0f);
            float angle1 = (i + 1) * angle_step - (angle_step * 2.0f);

            ImVec2 p0(center.x + cosf(angle0) * radius, center.y + sinf(angle0) * radius);
            ImVec2 p1(center.x + cosf(angle1) * radius, center.y + sinf(angle1) * radius);

            // Desenează segmentul de cerc
            if (i == colored_segment_index)
            {
                // Dacă este segmentul colorat, colorează întregul segment și feliile sale
                draw_list->AddLine(p0, p1, color, thickness);

                // Desenează feliile din interiorul segmentului colorat
                ImVec2 slice_center(center.x + cosf((angle0 + angle1) * 0.5f) * (radius * 0.5f), center.y + sinf((angle0 + angle1) * 0.5f) * (radius * 0.5f));
                ImVec2 slice_left(center.x + cosf(angle0) * (radius * 0.1f), center.y + sinf(angle0) * (radius * 0.1f));
                ImVec2 slice_right(center.x + cosf(angle1) * (radius * 0.1f), center.y + sinf(angle1) * (radius * 0.1f));

                // Desenează triunghiul umplut cu punctele în ordine corectă
                draw_filled_circle_segment(draw_list, center, radius, thickness, angle0, angle1, color);
            }
            else
            {
                // Altfel, desenează segmentul cu culoarea implicită
                draw_list->AddLine(p0, p1, IM_COL32_WHITE, thickness);
            }
        }
    }

    void draw_colored_segment(ImDrawList *draw_list, ImVec2 center, float radius, float thickness, float start_angle, float end_angle, ImU32 color, int num_segments)
    {
        float angle_step = (end_angle - start_angle) / num_segments;

        // Desenează segmentul de cerc principal
        draw_list->PathArcTo(center, radius, start_angle, end_angle, num_segments);
        draw_list->PathStroke(color, false, thickness);

        // Desenează unele linii din segment cu o altă culoare
        for (int i = num_segments; i < num_segments * 3 / 4; ++i)
        {
            float angle0 = start_angle + i * angle_step;
            float angle1 = start_angle + (i + 1) * angle_step;
            ImVec2 p0(center.x + cosf(angle0) * radius, center.y + sinf(angle0) * radius);
            ImVec2 p1(center.x + cosf(angle1) * radius, center.y + sinf(angle1) * radius);
            draw_list->AddLine(p0, p1, IM_COL32(255, 0, 0, 255), thickness);
        }
    }
    int draw_clock(ImDrawList *draw_list, ImVec2 center, int initial_angle, int second_angle, int segments, int labels_segments, ImVec4 selected_color, ImVec4 contur_color, ImVec4 clock_pointers_color, ImVec4 clock_axes_color)
    {

        float thickness = 2.5f;
        float start_angle = (initial_angle * IM_PI) / 180.0f;
        float end_angle = initial_angle + (second_angle * IM_PI) / 180.0f; // Exemplu: segment de la 0 la 90 de grade
        ImU32 segment_color = IM_COL32(selected_color.x * 255, selected_color.y * 255, selected_color.z * 255, selected_color.w * 255);
        // Desenează un segment de cerc umplut
        draw_filled_circle_segment(draw_list, ImVec2(center.x + radius, center.y + radius), radius, thickness, start_angle, end_angle, segment_color);

        ImU32 clock_contur_color = IM_COL32(contur_color.x * 255, contur_color.y * 255, contur_color.z * 255, contur_color.w * 255);
        draw_circle_with_axes(draw_list, ImVec2(center.x + radius, center.y + radius), radius, clock_contur_color, segments, 6.0f);
        ImU32 clk_axes_color = IM_COL32(clock_axes_color.x * 255, clock_axes_color.y * 255, clock_axes_color.z * 255, clock_axes_color.w * 255);
        draw_circle_with_axes_and_separate_labels(draw_list, ImVec2(center.x + radius, center.y + radius), radius, clk_axes_color, labels_segments, 0.5f);

        ImU32 clk_pointers_color = IM_COL32(clock_pointers_color.x * 255, clock_pointers_color.y * 255, clock_pointers_color.z * 255, clock_pointers_color.w * 255);
        // Desenare linie cu unghi dat - minutar
        draw_line_with_angle(draw_list, ImVec2(center.x + radius, center.y + radius), radius - 5, (float)(current_minute - 15) * IM_PI / 30.0, clk_pointers_color, 7.0f);
        // Desenare linie cu unghi dat - limba orei
        draw_line_with_angle(draw_list, ImVec2(center.x + radius, center.y + radius), radius - 20, (float)(current_hour - 15) * (2.0 * IM_PI / 12.0), clk_pointers_color, 7.0f);
        // Desenare linie cu unghi dat - secundar
        draw_line_with_angle(draw_list, ImVec2(center.x + radius, center.y + radius), radius + 2, (float)(current_second - 15) * (IM_PI / 30.0), clk_pointers_color, 3.0f);
        
        ImU32 extra_contour_color = IM_COL32(selected_color.x * 255, selected_color.y * 255, selected_color.z * 255, selected_color.w * 255);
        draw_colored_segment(ImGui::GetWindowDrawList(), ImVec2(center.x + radius, center.y + radius), radius + 10, thickness, start_angle, end_angle, extra_contour_color, segments);

        if (show_text)
        {
            // Afiseaza culoarea selectată
            ImGui::Text("Culoarea selectata: (%.2f, %.2f, %.2f, %.2f)", selected_color.x, selected_color.y, selected_color.z, selected_color.w);
            ImGui::Text("Culoarea selectata: (%.2f, %.2f, %.2f, %.2f)", contur_color.x, contur_color.y, contur_color.z, contur_color.w);
            ImGui::Text("Culoarea selectata: (%.2f, %.2f, %.2f, %.2f)", clock_axes_color.x, clock_axes_color.y, clock_axes_color.z, clock_axes_color.w);
            ImGui::Text("Culoarea selectata: (%.2f, %.2f, %.2f, %.2f)", clock_pointers_color.x, clock_pointers_color.y, clock_pointers_color.z, clock_pointers_color.w);
        }
        draw_current_time();
        return 0;
    }

    int draw_turbine(ImDrawList *draw_list, ImVec2 center, int initial_angle, int second_angle, int segments, int labels_segments, ImVec4 selected_color, ImVec4 contur_color, ImVec4 clock_pointers_color, ImVec4 clock_axes_color)
    {

        float thickness = 2.5f;
        float start_angle = (initial_angle * IM_PI) / 180.0f;
        float end_angle = initial_angle + (second_angle * IM_PI) / 180.0f; // Exemplu: segment de la 0 la 90 de grade
        ImU32 segment_color = IM_COL32(selected_color.x * 255, selected_color.y * 255, selected_color.z * 255, selected_color.w * 255);
        if(index>=segments)
            index = 0;
        // Desenează un segment de cerc umplut
        ImU32 extra_contour_color = IM_COL32(selected_color.x * 255, selected_color.y * 255, selected_color.z * 255, selected_color.w * 255);
        draw_colored_circle_segment(draw_list, ImVec2(center.x + radius, center.y + radius), radius, thickness, segments, index, extra_contour_color);

        ImU32 clock_contur_color = IM_COL32(contur_color.x * 255, contur_color.y * 255, contur_color.z * 255, contur_color.w * 255);
        draw_circle_with_axes(draw_list, ImVec2(center.x + radius, center.y + radius), radius, clock_contur_color, segments, 6.0f);
        ImU32 clk_axes_color = IM_COL32(clock_axes_color.x * 255, clock_axes_color.y * 255, clock_axes_color.z * 255, clock_axes_color.w * 255);
        draw_circle_with_axes_and_labels_with_font(draw_list, ImVec2(center.x + radius, center.y + radius), radius, clk_axes_color, segments, 0.5f);
        ImU32 clk_pointers_color = IM_COL32(clock_pointers_color.x * 255, clock_pointers_color.y * 255, clock_pointers_color.z * 255, clock_pointers_color.w * 255);


        if (show_text)
        {
            // Afiseaza culoarea selectată
            ImGui::Text("Culoarea selectata: (%.2f, %.2f, %.2f, %.2f)", selected_color.x, selected_color.y, selected_color.z, selected_color.w);
            ImGui::Text("Culoarea selectata: (%.2f, %.2f, %.2f, %.2f)", contur_color.x, contur_color.y, contur_color.z, contur_color.w);
            ImGui::Text("Culoarea selectata: (%.2f, %.2f, %.2f, %.2f)", clock_axes_color.x, clock_axes_color.y, clock_axes_color.z, clock_axes_color.w);
            ImGui::Text("Culoarea selectata: (%.2f, %.2f, %.2f, %.2f)", clock_pointers_color.x, clock_pointers_color.y, clock_pointers_color.z, clock_pointers_color.w);
        }
        draw_current_time();
        // Show values 
        ImVec2 cursor_pos = ImVec2(ImGui::GetWindowPos().x + 420 , ImGui::GetWindowPos().y + 520);
        char label[120];
        sprintf(label, "Rotation Speed: %.2f", rotation_speed); // Convertește numărul la șir de caractere
        draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(), cursor_pos, ImGui::GetColorU32(ImGuiCol_Text), label);

        cursor_pos.y += 30;
        sprintf(label, "Blade Index: %d", blade_index+1); // Convertește numărul la șir de caractere
        draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(), cursor_pos, ImGui::GetColorU32(ImGuiCol_Text), label);
        cursor_pos.y += 30;
        sprintf(label, "Current Vibration: %.2f", current_vibration); // Convertește numărul la șir de caractere
        draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(), cursor_pos, ImGui::GetColorU32(ImGuiCol_Text), label);

        return 0;
    }
};