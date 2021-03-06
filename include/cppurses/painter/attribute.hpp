#ifndef PAINTER_ATTRIBUTE_HPP
#define PAINTER_ATTRIBUTE_HPP
#include <cstdint>

namespace cppurses {

enum class Attribute : std::int8_t {
    Bold,
    Italic,
    Underline,
    Standout,
    Dim,
    Inverse,
    Invisible,
    Blink
};

}  // namespace cppurses
#endif  // PAINTER_ATTRIBUTE_HPP
