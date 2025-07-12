#ifndef MAIN_H
#define MAIN_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/geometry2d.hpp>
#include <godot_cpp/classes/rendering_server.hpp>

#include "personal_macro.h"

#define MAX_JOINTS 100

namespace godot
{

class Main : public Node2D
{
    GDCLASS(Main, Node2D);

private:
    Color outline_color = Color(0.8f, 0.9f, 1.0f, 1.0f);
    Color fill_color = Color(0.4f, 0.9f, 0.8f, 1.0f);
    Color vertex_color = Color(1.0f, 0.0f, 0.0f, 1.0f);

    bool show_vertices = false;
    bool show_outline = false;
    bool use_snake_length = true;

    uint8_t joint_count = 20;
    uint8_t vertex_count = (joint_count + 3) * 2;
    uint8_t index_count = (vertex_count - 2) * 3;

    float angle_coeff = 1.0f;
    float snake_length = 30.0f;
    float joint_radius = snake_length / joint_count;

    float head_size = 50.0f;
    float radius_scale = 10.0f;
    float scale_power = 1.0f;
    float joint_sizes[MAX_JOINTS];

    Vector2 snake_nose;
    Vector2 snake_head;
    Vector2 joints[MAX_JOINTS];
    PackedVector2Array vertices;
    PackedInt32Array indices;

    Viewport* viewport;
    RID canvas_item;
    RenderingServer* rendering_server;

protected:
    static void _bind_methods();

public:
    void _ready() override;
    void _process(double delta) override;
    void _draw() override;
    void _input(const Ref<InputEvent>& p_event) override;

    inline void calculate_vertices();
    inline void recalculate_sizes();
    inline void draw_traingles();

    DECLARE_PROPERTY(outline_color, Color);
    DECLARE_PROPERTY(fill_color, Color);
    DECLARE_PROPERTY(vertex_color, Color);

    DECLARE_PROPERTY(show_vertices, bool);
    DECLARE_PROPERTY(show_outline, bool);

    DECLARE_PROPERTY(use_snake_length, bool);
    DECLARE_PROPERTY(joint_count, uint8_t);
    DECLARE_PROPERTY(snake_length, float);

    DECLARE_PROPERTY(angle_coeff, float);
    DECLARE_PROPERTY(head_size, float);
    DECLARE_PROPERTY(radius_scale, float);
    DECLARE_PROPERTY(scale_power, float);
};

}

#endif