#include "main.h"

#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/input_event_mouse.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/global_constants.hpp>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/math.hpp>

using namespace godot;
using namespace Math;

void Main::_bind_methods()
{
    EXPORT_PROPERTY(fill_color, COLOR, Main);
    EXPORT_PROPERTY(fins_color, COLOR, Main);
    EXPORT_PROPERTY(vertex_color, COLOR, Main);
    EXPORT_PROPERTY(outline_color, COLOR, Main);
    EXPORT_PROPERTY(eyes_color, COLOR, Main);

    EXPORT_PROPERTY(show_vertices, BOOL, Main);
    EXPORT_PROPERTY(show_outline, BOOL, Main);

    EXPORT_PROPERTY(joint_count, INT, Main);
    EXPORT_PROPERTY(shape_length, FLOAT, Main);
    EXPORT_PROPERTY(shape_width, FLOAT, Main);
    EXPORT_PROPERTY(outline_width, FLOAT, Main);
    EXPORT_PROPERTY(angle_coeff, FLOAT, Main);

    EXPORT_PROPERTY(radius_power, FLOAT, Main);
    EXPORT_PROPERTY(radius_shift, FLOAT, Main);
    EXPORT_PROPERTY(radius_coeff, FLOAT, Main);

    EXPORT_PROPERTY(size_power, FLOAT, Main);
    EXPORT_PROPERTY(size_shift, FLOAT, Main);
    EXPORT_PROPERTY(size_coeff, FLOAT, Main);
}

void Main::_ready()
{
    screen_size = get_viewport()->get_visible_rect().size;
    head_position = screen_size / 2.0f;

    canvas_item = get_canvas_item();
    rendering_server = RenderingServer::get_singleton();

    recalculate_radiuses();
}

void Main::_input(const Ref<InputEvent>& p_event)
{
    Ref<InputEventMouse> mouse_input = p_event;
    if (mouse_input != nullptr && mouse_input->get_button_mask() == 1)
    {
        head_position = mouse_input->get_global_position();
    }

    Ref<InputEventKey> key_input = p_event;
    if (key_input != nullptr && !key_input->is_echo())
    {
        if (key_input->get_keycode() == KEY_W)
        {
            if (key_input->is_pressed()) velocity.y -= 1.0f;
            else velocity.y += 1.0f; 
        }

        if (key_input->get_keycode() == KEY_S)
        {
            if (key_input->is_pressed()) velocity.y += 1.0f;
            else velocity.y -= 1.0f; 
        }

        if (key_input->get_keycode() == KEY_A)
        {
            if (key_input->is_pressed()) velocity.x -= 1.0f;
            else velocity.x += 1.0f; 
        }

        if (key_input->get_keycode() == KEY_D)
        {
            if (key_input->is_pressed()) velocity.x += 1.0f;
            else velocity.x -= 1.0f; 
        }
    }
}

void Main::_process(double delta)
{
    move(delta);
    calculate_shape();
    draw_traingles();
}

inline void Main::move(double delta)
{
    head_position += velocity.normalized() * speed * (float)delta;
    head_position.x = clamp(head_position.x, 0.0f, screen_size.x);
    head_position.y = clamp(head_position.y, 0.0f, screen_size.y);
}

inline void Main::calculate_shape()
{
    Vector2 direction = (joints[0] - head_position).normalized() * sizes[0];
    Vector2 orth_direction(direction.y, direction.x * -1.0f);
    Vector2 current_joint = head_position + direction * 1.1f;

    calculate_head(direction, orth_direction, current_joint);
    calculate_body(direction, orth_direction, current_joint);
    calculate_tail(direction, orth_direction, current_joint);
}

inline void Main::calculate_head(Vector2 &direction, Vector2 &orth_direction, Vector2 &current_joint)
{
    const float inbetween_angle = (float)Math_PI / 3.0f;
    Vector2 inbetween_direction_v = cos(inbetween_angle) * orth_direction * 1.1f;
    Vector2 inbetween_direction_w = sin(inbetween_angle) * direction * 1.1f;

    vertices[0] = head_position;
    vertices[1] = current_joint - inbetween_direction_v - inbetween_direction_w;
    vertices[2] = current_joint + inbetween_direction_v - inbetween_direction_w;

    eyes[0] = current_joint - orth_direction / 1.5f;
    eyes[1] = current_joint + orth_direction / 1.5f;


    current_joint = head_position;
    direction /= sizes[0];
    inbetween_direction_v /= sizes[0];
    inbetween_direction_w /= sizes[0];

    outline[0] = vertices[0] - direction * outline_width;
    outline[1] = vertices[1] - (inbetween_direction_v + inbetween_direction_w) * outline_width;
    outline[2] = vertices[2] + (inbetween_direction_v - inbetween_direction_w) * outline_width;

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
}

inline void Main::calculate_body(Vector2 &direction, Vector2 &orth_direction, Vector2 &current_joint)
{
    for (uint32_t i = 0; i < joint_count; ++i)
    {
        Vector2 difference = joints[i] - current_joint;

        float vector_angle = direction.angle_to(difference * -1.0f);
        float radius_ratio = sizes[i] / radiuses[i]  * angle_coeff;
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

        orth_direction = Vector2(direction.y * sizes[i], direction.x * sizes[i] * -1.0f);
        current_joint += direction * radiuses[i];
        joints[i] = current_joint;


        uint32_t j = i * 2;
        vertices[j + 3] = current_joint - orth_direction;
        vertices[j + 4] = current_joint + orth_direction;

        orth_direction /= sizes[i];
        outline[j + 3] = vertices[j + 3] - orth_direction * outline_width;
        outline[j + 4] = vertices[j + 4] + orth_direction * outline_width;


        uint32_t k = i * 6;
        indices[k + 3] = j + 3;
        indices[k + 4] = j + 2;
        indices[k + 5] = j + 1;

        indices[k + 6] = j + 2;
        indices[k + 7] = j + 3;
        indices[k + 8] = j + 4;
    }
}

inline void Main::calculate_tail(Vector2 &direction, Vector2 &orth_direction, Vector2 &current_joint)
{
    direction *= sizes[joint_count - 1];

    const float inbetween_angle = (float)Math_PI / 4.0f;
    Vector2 inbetween_direction_v = cos(inbetween_angle) * orth_direction * sizes[joint_count - 1];
    Vector2 inbetween_direction_w = sin(inbetween_angle) * direction;

    vertices[vertex_count - 3] = current_joint - (inbetween_direction_v - inbetween_direction_w);
    vertices[vertex_count - 2] = current_joint + (inbetween_direction_v + inbetween_direction_w);
    vertices[vertex_count - 1] = current_joint + direction * 1.1f;


    direction /= sizes[joint_count - 1];
    inbetween_direction_v /= sizes[joint_count - 1];
    inbetween_direction_w /= sizes[joint_count - 1];

    outline[vertex_count - 3] = vertices[vertex_count - 3] - (inbetween_direction_v - inbetween_direction_w) * outline_width;
    outline[vertex_count - 2] = vertices[vertex_count - 2] + (inbetween_direction_v + inbetween_direction_w) * outline_width;
    outline[vertex_count - 1] = vertices[vertex_count - 1] + direction * outline_width;


    indices[index_count - 9] = vertex_count - 2;
    indices[index_count - 8] = vertex_count - 4;
    indices[index_count - 7] = vertex_count - 5;

    indices[index_count - 6] = vertex_count - 5;
    indices[index_count - 5] = vertex_count - 3;
    indices[index_count - 4] = vertex_count - 2;

    indices[index_count - 3] = vertex_count - 1;
    indices[index_count - 2] = vertex_count - 2;
    indices[index_count - 1] = vertex_count - 3;
}

inline void draw_ellipse(RenderingServer* rendering_server, const RID &canvas_item, const Vector2 &center, float rotation_angle, float radius_x, float radius_y, uint32_t segments, Color color)
{
    PackedColorArray colors = { color };
    PackedVector2Array points;
    points.resize(segments);

    Vector2 rotation_x(cos(rotation_angle), sin(rotation_angle));
    Vector2 rotation_y(rotation_x.y * -1.0f, rotation_x.x);

    float angle = 0.0f;
    float step = (float)Math_TAU / (float)segments;

    for (uint32_t i = 0; i < segments; ++i)
    {
        Vector2 x = cos(angle) * radius_x * rotation_x;
        Vector2 y = sin(angle) * radius_y * rotation_y;

        points[i] = center + x + y;
        angle += step;
    }

    rendering_server->canvas_item_add_polygon(canvas_item, points, colors);
}

inline void Main::draw_traingles()
{
    rendering_server->canvas_item_clear(canvas_item);

    draw_ellipse(rendering_server, canvas_item, vertices[4 + joint_count / 3], (float)Math_PI / -4.0f, 30.0f, 10.0f, 16, fins_color);
    draw_ellipse(rendering_server, canvas_item, vertices[5 + joint_count / 3], (float)Math_PI / 4.0f, 30.0f, 10.0f, 16, fins_color);
    draw_ellipse(rendering_server, canvas_item, vertices[5 + joint_count], (float)Math_PI / -4.0f, 15.0f, 5.0f, 16, fins_color);
    draw_ellipse(rendering_server, canvas_item, vertices[6 + joint_count], (float)Math_PI / 4.0f, 15.0f, 5.0f, 16, fins_color);

    PackedColorArray colors = { fill_color };
    PackedColorArray outline_color = { outline_color };


    if (show_outline)
        rendering_server->canvas_item_add_triangle_array(canvas_item, indices, outline, outline_color);

    rendering_server->canvas_item_add_triangle_array(canvas_item, indices, vertices, colors);
    rendering_server->canvas_item_add_circle(canvas_item, eyes[0], sizes[0] / 4.5f, eyes_color);
    rendering_server->canvas_item_add_circle(canvas_item, eyes[1], sizes[0] / 4.5f, eyes_color);

    if (show_vertices)
    {
        for (uint32_t i = 0; i < vertex_count; ++i)
        {
            rendering_server->canvas_item_add_circle(canvas_item, vertices[i], 2.0f, vertex_color);
        }
    }
}

inline void Main::recalculate_radiuses()
{
    float radius_sum = 0.0f;

    for (uint32_t i = 0; i < joint_count; ++i)
    {
        radiuses[i] = 1.0f / (powf((float)i * radius_coeff + radius_shift, radius_power) + 1.0f);
        sizes[i] = shape_width / (powf((float)i * size_coeff + size_shift, size_power) + 1.0f);

        radius_sum += radiuses[i];
    }

    float radius_normal = shape_length / radius_sum;

    for (uint32_t i = 0; i < joint_count; ++i)
    {
        radiuses[i] *= radius_normal;
    }
}

DEFINE_PROPERTY(fill_color, Color, Main, )
DEFINE_PROPERTY(fins_color, Color, Main, )
DEFINE_PROPERTY(vertex_color, Color, Main, )
DEFINE_PROPERTY(outline_color, Color, Main, )
DEFINE_PROPERTY(eyes_color, Color, Main, )

DEFINE_PROPERTY(show_vertices, bool, Main, )
DEFINE_PROPERTY(show_outline, bool, Main, )

DEFINE_PROPERTY(joint_count, uint32_t, Main, 
    if (joint_count < 1) joint_count = 1; 
    if (joint_count > MAX_JOINTS) joint_count = MAX_JOINTS; 

    joint_radius = shape_length / joint_count;
    vertex_count = (joint_count + 3) * 2; 
    index_count = (vertex_count - 2) * 3;

    vertices.resize(vertex_count);
    indices.resize(index_count);
    outline.resize(vertex_count);

    recalculate_radiuses();
)

DEFINE_PROPERTY(shape_length, float, Main, recalculate_radiuses();)
DEFINE_PROPERTY(shape_width, float, Main, recalculate_radiuses();)
DEFINE_PROPERTY(outline_width, float, Main, recalculate_radiuses();)
DEFINE_PROPERTY(angle_coeff, float, Main, )

DEFINE_PROPERTY(radius_power, float, Main, recalculate_radiuses();)
DEFINE_PROPERTY(radius_shift, float, Main, recalculate_radiuses();)
DEFINE_PROPERTY(radius_coeff, float, Main, recalculate_radiuses();)

DEFINE_PROPERTY(size_power, float, Main, recalculate_radiuses();)
DEFINE_PROPERTY(size_shift, float, Main, recalculate_radiuses();)
DEFINE_PROPERTY(size_coeff, float, Main, recalculate_radiuses();)