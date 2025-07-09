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
    print_line(Vector2(0.0f, 1.0f).angle_to(Vector2(1.0f, 0.0f)));
    viewport = get_viewport();
    screen_size = viewport->get_visible_rect().size;
    screen_center = screen_size / 2.0f;
}

void Main::_draw()
{
    draw_line(points[0], points[point_count - 1], outline_color, 1.0f, true);
    for (uint8_t i = 1; i < point_count; ++i)
    {
        draw_line(points[i - 1], points[i], outline_color, 1.0f, true);
    }

    for (uint8_t i = 0; i < point_count; ++i)
    {
        draw_circle(points[i], 5.0f, Color(1.0f, 0.0f, 0.0f, 2.0f), true, -1.0f, true);
    }
}

void Main::_process(double delta)
{
    Vector2 front_point = viewport->get_mouse_position();
    points[point_count - 1] = front_point;

    Vector2 direction = joints[0] - front_point;
    for (uint8_t i = 0; i < joint_count; ++i)
    {
        Vector2 difference = joints[i] - front_point;
        float vector_angle = direction.angle_to(difference * -1.0f);

        float angle_shift = abs(vector_angle) - min_angle;
        if (angle_shift < 0.0f)
        {
            vector_angle = difference.angle() - angle_shift * sign(vector_angle);
            direction = Vector2(cos(vector_angle), sin(vector_angle));
        }
        else
        {
            direction = difference.normalized();
        }

        direction *= joint_radius / 2.0f;
        front_point += direction;

        points[i].x = front_point.x + direction.y;
        points[i].y = front_point.y - direction.x;
        points[point_count - i - 2].x = front_point.x - direction.y;
        points[point_count - i - 2].y = front_point.y + direction.x;

        front_point += direction;
        joints[i] = front_point;
    }
    points[joint_count] = front_point;

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