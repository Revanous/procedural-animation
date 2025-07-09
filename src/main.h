#ifndef MAIN_H
#define MAIN_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/input_event.hpp>

#define MAX_JOINTS 5
#define MAX_POINTS ((MAX_JOINTS + 1) * 2)

namespace godot
{

class Main : public Node2D
{
    GDCLASS(Main, Node2D);

private:
    const uint8_t joint_count = MAX_JOINTS;
    const uint8_t point_count = MAX_POINTS;
    const float joint_radius = 100.0f;
    const Color outline_color = Color(0.4f, 0.9f, 0.8f, 1.0f);

    Viewport* viewport;

    Vector2 screen_size;
    Vector2 screen_center;
    Vector2 joints[MAX_JOINTS];
    Vector2 points[MAX_POINTS];
    Vector2 visual_joints[MAX_JOINTS + 1];

protected:
    static void _bind_methods();

public:
    void _ready() override;
    void _draw() override;
    void _process(double delta) override;
    // void _input(const Ref<InputEvent>& p_event) override;
};

}

#endif