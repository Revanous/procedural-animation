#ifndef MAIN_H
#define MAIN_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include "personal_macro.h"

#define MAX_JOINTS 100
#define GET_POINT_COUNT(joint_count) ((joint_count + 1) * 2)

namespace godot
{

class Main : public Node2D
{
    GDCLASS(Main, Node2D);

private:
    const float min_angle = (float)Math_PI * 2.0f / 3.0f;
    const Color outline_color = Color(0.4f, 0.9f, 0.8f, 1.0f);
    uint8_t point_count;
    Vector2 head;

    float radiuses[MAX_JOINTS];
    Vector2 joints[MAX_JOINTS];
    Vector2 points[GET_POINT_COUNT(MAX_JOINTS)];

    Viewport* viewport;

protected:
    static void _bind_methods();

public:
    void _ready() override;
    void _draw() override;
    void _process(double delta) override;
    inline void recalculate_radiuses();
    void _input(const Ref<InputEvent>& p_event) override;

    DECLARE_PROPERTY(show_points, bool);
    DECLARE_PROPERTY(joint_count, uint8_t);
    DECLARE_PROPERTY(joint_radius, float);
    DECLARE_PROPERTY(radius_scale, float);
    DECLARE_PROPERTY(scale_power, uint8_t);
};

}

#endif