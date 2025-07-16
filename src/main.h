#ifndef MAIN_H
#define MAIN_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/rendering_server.hpp>

#include "personal_macro.h"

#define MAX_JOINTS 100

namespace godot
{

class Main : public Node2D
{
    GDCLASS(Main, Node2D);

private:
    Color fill_color = Color(0.4f, 0.9f, 0.8f, 1.0f);
    Color fins_color = Color(0.4f, 0.9f, 0.8f, 1.0f);
    Color vertex_color = Color(1.0f, 0.0f, 0.0f, 1.0f);
    Color outline_color = Color(0.9f, 0.9f, 0.9f, 1.0f);
    Color eyes_color = Color(0.9f, 0.9f, 0.9f, 1.0f);

    bool show_vertices = false;
    bool show_outline = false;

    uint32_t joint_count;
    uint32_t vertex_count;
    uint32_t index_count;

    float angle_coeff = 4.0f;
    float shape_length = 15.0f;
    float shape_width = 15.0f;
    float outline_width = 1.2f;
    float joint_radius = shape_length / joint_count;

    float radius_power = 1.0f;
    float radius_shift = 2.0f;
    float radius_coeff = 1.0f;
    float radiuses[MAX_JOINTS];

    float size_power = 1.0f;
    float size_shift = 2.0f;
    float size_coeff = 1.0f;
    float sizes[MAX_JOINTS];

    float speed = 500.0f;
    Vector2 head_position;
    Vector2 velocity;
    Vector2 screen_size;

    Vector2 joints[MAX_JOINTS];
    Vector2 eyes[2];
    Vector2 vertex_sum;
    PackedVector2Array vertices;
    PackedVector2Array outline;
    PackedInt32Array indices;

    RID canvas_item;
    RenderingServer* rendering_server;

protected:
    static void _bind_methods();

public:
    void _ready() override;
    void _process(double delta) override;
    void _input(const Ref<InputEvent>& p_event) override;

    inline void move(double delta);
    inline void calculate_shape();
    inline void calculate_head(Vector2 &direction, Vector2 &orth_direction, Vector2 &current_joint);
    inline void calculate_body(Vector2 &direction, Vector2 &orth_direction, Vector2 &current_joint);
    inline void calculate_tail(Vector2 &direction, Vector2 &orth_direction, Vector2 &current_joint);

    inline void recalculate_radiuses();
    inline void draw_traingles();

    DECLARE_PROPERTY(fill_color, Color);
    DECLARE_PROPERTY(fins_color, Color);
    DECLARE_PROPERTY(vertex_color, Color);
    DECLARE_PROPERTY(outline_color, Color);
    DECLARE_PROPERTY(eyes_color, Color);

    DECLARE_PROPERTY(show_vertices, bool);
    DECLARE_PROPERTY(show_outline, bool);

    DECLARE_PROPERTY(joint_count, uint32_t);
    DECLARE_PROPERTY(shape_length, float);
    DECLARE_PROPERTY(shape_width, float);
    DECLARE_PROPERTY(outline_width, float);

    DECLARE_PROPERTY(angle_coeff, float);
    DECLARE_PROPERTY(radius_power, float);
    DECLARE_PROPERTY(radius_shift, float);
    DECLARE_PROPERTY(radius_coeff, float);

    DECLARE_PROPERTY(size_power, float);
    DECLARE_PROPERTY(size_shift, float);
    DECLARE_PROPERTY(size_coeff, float);
};

}

#endif