#ifndef MAIN_H
#define MAIN_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/input_event.hpp>

#define MAX_POINTS 5

namespace godot
{

class Main : public Node2D
{
    GDCLASS(Main, Node2D);

private:
    const Color point_color = Color(0.4f, 0.9f, 0.8f, 1.0f);
    const float circle_radius = 5.0f;
    const float point_radius = circle_radius + 55.0f;

    uint8_t point_count = MAX_POINTS;
    Viewport* viewport;

    Vector2 screen_size;
    Vector2 screen_center;
    Vector2 points[MAX_POINTS];

protected:
    static void _bind_methods();

public:
    void _ready() override;
    void _draw() override;
    void _process(double delta) override;
    void _input(const Ref<InputEvent>& p_event) override;
};

}

#endif