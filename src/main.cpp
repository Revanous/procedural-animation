#include "main.h"

#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/input_event_mouse.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/math.hpp>

using namespace godot;
using namespace Math;

void Main::_bind_methods()
{
    EXPORT_PROPERTY(show_points, BOOL, Main);
    EXPORT_PROPERTY(use_snake_length, BOOL, Main);
    EXPORT_PROPERTY(joint_count, INT, Main);
    EXPORT_PROPERTY(snake_length, FLOAT, Main);

    EXPORT_PROPERTY(angle_coeff, FLOAT, Main);
    EXPORT_PROPERTY(head_size, FLOAT, Main);
    EXPORT_PROPERTY(radius_scale, FLOAT, Main);
    EXPORT_PROPERTY(scale_power, FLOAT, Main);
}

void Main::_ready()
{
    viewport = get_viewport();
    canvas_item = get_canvas_item();
    recalculate_sizes();
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
            draw_circle(points[i], joint_sizes[i / joint_count] / 10.0f, Color(1.0f, 0.0f, 0.0f, 2.0f), true, -1.0f, true);
        }
    }
}

void Main::_input(const Ref<InputEvent>& p_event)
{
    Ref<InputEventMouse> mouse_input = p_event;
    if (mouse_input != nullptr && mouse_input->get_button_mask() == 1)
    {
        snake_nose = viewport->get_mouse_position();
        points[0] = snake_nose;
    }
}

void Main::_process(double delta)
{
    Vector2 direction = snake_head - snake_nose;
    direction = direction.normalized() * head_size;
    Vector2 orth_direction(direction.y, direction.x * -1.0f);

    Vector2 front_point = snake_nose + direction;
    snake_head = front_point;

    const float cheek_angle = (float)Math_PI / 3.0f;
    Vector2 cheek_direction_v = cos(cheek_angle) * orth_direction;
    Vector2 cheek_direction_w = sin(cheek_angle) * direction;

    points[1] = front_point + cheek_direction_v - cheek_direction_w;
    points[point_count - 1] = front_point - cheek_direction_v - cheek_direction_w;
    points[point_count - 2] = front_point - orth_direction;
    points[2] = front_point + orth_direction;

    for (uint8_t i = 0; i < joint_count; ++i)
    {
        Vector2 difference = joints[i] - front_point;
        float vector_angle = direction.angle_to(difference * -1.0f);

        float radius_ratio = joint_sizes[i] * angle_coeff;
        if (use_snake_length)
            radius_ratio /= joint_radius;

        float angle_shift = abs(vector_angle) - (float)Math_PI * radius_ratio / (radius_ratio + 1.0f);
        if (angle_shift < 0.0f)
        {
            vector_angle = difference.angle() - angle_shift * sign(vector_angle);
            direction = Vector2(cos(vector_angle), sin(vector_angle));
        }
        else
        {
            direction = difference.normalized();
        }
        orth_direction = Vector2(direction.y * joint_sizes[i], direction.x * joint_sizes[i] * -1.0f);

        if (use_snake_length)
            front_point += direction * joint_radius;
        else
            front_point += direction * joint_sizes[i];

        points[i + 3] = front_point + orth_direction;
        points[point_count - i - 3]= front_point - orth_direction;
        joints[i] = front_point;
    }
    points[joint_count + 3] = Vector2(front_point.x - orth_direction.y, front_point.y + orth_direction.x);

    queue_redraw();
}

inline void Main::recalculate_sizes()
{
    for (uint8_t i = 0; i < joint_count; ++i)
    {
        joint_sizes[i] = head_size / (pow((float)i, scale_power) * radius_scale + 1.0f);
    }
}

DEFINE_PROPERTY(show_points, bool, Main, )
DEFINE_PROPERTY(use_snake_length, bool, Main, )
DEFINE_PROPERTY(joint_count, uint8_t, Main, 
    if (joint_count < 0) joint_count = 0; 
    if (joint_count > 100) joint_count = 100; 
    joint_radius = snake_length / joint_count;
    point_count = GET_POINT_COUNT(joint_count); 
    recalculate_sizes();
)
DEFINE_PROPERTY(snake_length, float, Main, joint_radius = snake_length / joint_count;)
DEFINE_PROPERTY(angle_coeff, float, Main, )
DEFINE_PROPERTY(head_size, float, Main, recalculate_sizes();)
DEFINE_PROPERTY(radius_scale, float, Main, recalculate_sizes();)
DEFINE_PROPERTY(scale_power, float, Main, recalculate_sizes();)