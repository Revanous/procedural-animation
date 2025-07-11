#ifndef PERSONAL_MACRO_H
#define PERSONAL_MACRO_H

#define STR_HELPER(x) #x
#define STR(X) STR_HELPER(X)

#define DECLARE_PROPERTY(name, type)\
    void set_##name(const type name);\
    type get_##name() const

#define EXPORT_PROPERTY(name, type, class)\
    ClassDB::bind_method(D_METHOD("set_" STR(name), "p_" STR(name)), &class::set_##name);\
    ClassDB::bind_method(D_METHOD("get_" STR(name)), &class::get_##name);\
    ClassDB::add_property(STR(class), PropertyInfo(Variant::type, STR(name)), "set_" STR(name), "get_" STR(name))

#define DEFINE_PROPERTY(name, type, class, context)\
    void class::set_##name(const type p_##name) { name = p_##name; context }\
    type class::get_##name() const { return name; }

#endif