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
    draw_circle(visual_joints[0], 10.0f, outline_color, true, -1.0f, true);
    for (uint8_t i = 0; i < joint_count; ++i)
    {
        draw_circle(visual_joints[i + 1], 10.0f, outline_color, true, -1.0f, true);
        draw_line(visual_joints[i], visual_joints[i + 1], outline_color, 3.0f, true);
    }

    for (uint8_t i = 0; i < point_count; ++i)
    {
        draw_circle(points[i], 5.0f, Color(1.0f, 0.0f, 0.0f, 1.0f), true, -1.0f, true);
    }
}

void Main::_process(double delta)
{
    Vector2 front_point = viewport->get_mouse_position();
    points[point_count - 1] = front_point;
    visual_joints[0] = front_point;

    for (uint8_t i = 0; i < joint_count; ++i)
    {
        Vector2 difference = joints[i] - front_point;
        Vector2 direction = difference.normalized() * joint_radius / 2.0f;
        difference /= 2.0f;

        points[i].x = front_point.x + difference.x + direction.y;
        points[i].y = front_point.y + difference.y - direction.x;
        points[point_count - i - 2].x = front_point.x + difference.x - direction.y;
        points[point_count - i - 2].y = front_point.y + difference.y + direction.x;

        front_point += direction * 2.0f;
        joints[i] = front_point;
        visual_joints[i + 1] = front_point;
    }
    points[joint_count] = joints[joint_count - 1];

    queue_redraw();
}

// void Main::_input(const Ref<InputEvent>& p_event)
// {
//     Ref<InputEventMouse> mouse_input = p_event;
//     if (mouse_input != nullptr && mouse_input->get_button_mask() == 1)
//     {
//         points[0] = mouse_input->get_position();
//         for (uint8_t i = 1; i < point_count; ++i)
//         {
//             Vector2 difference = points[i] - points[i - 1];
//             points[i] = points[i - 1] + difference.normalized() * point_radius;
//         }
//         queue_redraw();
//     }
// }