#include "main.h"

#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/input_event_mouse.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/math.hpp>

using namespace godot;

void Main::_bind_methods()
{
    EXPORT_PROPERTY(show_points, BOOL, Main);
    EXPORT_PROPERTY(joint_count, INT, Main);
    EXPORT_PROPERTY(joint_radius, FLOAT, Main);
    EXPORT_PROPERTY(radius_scale, FLOAT, Main);
    EXPORT_PROPERTY(scale_power, INT, Main);
}

void Main::_ready()
{
    viewport = get_viewport();
}

void Main::_draw()
{
    draw_line(points[0], points[point_count - 1], outline_color, 1.0f, true);
    for (uint8_t i = 1; i < point_count; ++i)
    {
        draw_line(points[i - 1], points[i], outline_color, 1.0f, true);
    }

    if (show_points)
    {
        for (uint8_t i = 0; i < point_count; ++i)
        {
            draw_circle(points[i], 5.0f, Color(1.0f, 0.0f, 0.0f, 2.0f), true, -1.0f, true);
        }
    }
}

void Main::_input(const Ref<InputEvent>& p_event)
{
    Ref<InputEventMouse> mouse_input = p_event;
    if (mouse_input != nullptr && mouse_input->get_button_mask() == 1)
    {
        head = viewport->get_mouse_position();
    }
}

void Main::_process(double delta)
{
    Vector2 front_point = head;
    points[point_count - 1] = front_point;

    Vector2 direction = joints[0] - front_point;
    // direction = direction.normalized() * joint_radius / 2.0f;

    for (uint8_t i = 0; i < joint_count; ++i)
    {
        Vector2 difference = joints[i] - front_point;
        float vector_angle = direction.angle_to(difference * -1.0f);

        float angle_shift = Math::abs(vector_angle) - min_angle;
        if (angle_shift < 0.0f)
        {
            vector_angle = difference.angle() - angle_shift * Math::sign(vector_angle);
            direction = Vector2(Math::cos(vector_angle), Math::sin(vector_angle));
        }
        else
        {
            direction = difference.normalized();
        }

        direction *= radiuses[i] / 2.0f;
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

inline void Main::recalculate_radiuses()
{
    for (uint8_t i = 0; i < joint_count; ++i)
    {
        radiuses[i] = joint_radius / (Math::pow((float)i, (float)scale_power) / radius_scale + 1.0f);
    }
}

DEFINE_PROPERTY(show_points, bool, Main, )
DEFINE_PROPERTY(joint_count, uint8_t, Main, 
    if (joint_count < 0) joint_count = 0; 
    if (joint_count > 100) joint_count = 100; 
    point_count = GET_POINT_COUNT(joint_count); 
    recalculate_radiuses();
)
DEFINE_PROPERTY(joint_radius, float, Main, recalculate_radiuses();)
DEFINE_PROPERTY(radius_scale, float, Main, recalculate_radiuses();)
DEFINE_PROPERTY(scale_power, uint8_t, Main, recalculate_radiuses();)