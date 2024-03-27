# Simple_ImGui_Drawing

The provided example demonstrates how to utilize the Dear ImGui library to draw a circle within an ImGui docking window. Dear ImGui is a popular GUI library that allows for easy integration of graphical user interfaces into applications. The example showcases the drawing capabilities of ImGui by creating a simple window and drawing a circle inside it.

Key Components:

    ImGui Integration: The example initializes Dear ImGui, sets up the necessary ImGui context, and initializes ImGui for rendering using GLFW and OpenGL.

    Drawing a Circle: To draw a circle, the example defines a function draw_circle() that takes parameters such as the drawing list, center coordinates, radius, color, and number of segments to approximate the circle. This function utilizes ImGui's drawing API to add lines representing the circumference of the circle.

    Drawing the UI: Inside the ImGui window, the circle is drawn by calling the draw_circle() function with appropriate parameters, such as center coordinates, radius, and color.

    Rendering Loop: The example enters a rendering loop where ImGui frames are processed, ImGui UI is rendered, and GLFW buffers are swapped for display. The circle is continuously redrawn within the ImGui window as long as the application is running.

Overall, the example demonstrates how ImGui can be used to create custom graphical elements, such as circles, within an ImGui-based user interface, providing developers with a convenient way to incorporate interactive visuals into their applications.
