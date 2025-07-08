#include "main.h"

#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/input_event_mouse.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/core/math.hpp>

using namespace godot;
using namespace Math;

void Main::_bind_methods() {}

void Main::_ready()
{
    viewport = get_viewport();
    screen_size = viewport->get_visible_rect().size;
    screen_center = screen_size / 2.0f;
}

void Main::_draw()
{
    draw_circle(points[0], circle_radius, point_color, true, -1.0f, true);
    for (uint8_t i = 1; i < point_count; ++i)
    {
        draw_circle(points[i], circle_radius, point_color, true, -1.0f, true);
        draw_line(points[i - 1], points[i], point_color, -1.0f, true);
    }
}

void Main::_process(double delta)
{
    points[0] = viewport->get_mouse_position();

    for (uint8_t i = 1; i < point_count; ++i)
    {
        Vector2 difference = points[i] - points[i - 1];
        points[i] = points[i - 1] + difference.normalized() * point_radius;
    }
    queue_redraw();
}

void Main::_input(const Ref<InputEvent>& p_event)
{
    // Ref<InputEventMouse> mouse_input = p_event;
    // if (mouse_input != nullptr && mouse_input->get_button_mask() == 1)
    // {
    //     points[0] = mouse_input->get_position();
    //     for (uint8_t i = 1; i < point_count; ++i)
    //     {
    //         Vector2 difference = points[i] - points[i - 1];
    //         points[i] = points[i - 1] + difference.normalized() * point_radius;
    //     }
    //     queue_redraw();
    // }
}