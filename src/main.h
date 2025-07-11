#ifndef MAIN_H
#define MAIN_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include "personal_macro.h"

#define MAX_JOINTS 100
#define GET_POINT_COUNT(joint_count) ((joint_count + 3) * 2)

namespace godot
{

class Main : public Node2D
{
    GDCLASS(Main, Node2D);

private:
    const Color outline_color = Color(0.4f, 0.9f, 0.8f, 1.0f);
    bool show_points = false;
    bool use_snake_length = true;

    uint8_t joint_count = 20;
    uint8_t point_count = GET_POINT_COUNT(joint_count);

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
    Vector2 points[GET_POINT_COUNT(MAX_JOINTS)];

    Viewport* viewport;

protected:
    static void _bind_methods();

public:
    void _ready() override;
    void _draw() override;
    void _process(double delta) override;
    inline void recalculate_sizes();
    void _input(const Ref<InputEvent>& p_event) override;

    DECLARE_PROPERTY(show_points, bool);
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