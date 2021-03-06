#ifndef WIDGET_WIDGETS_TEXTBOX_HPP
#define WIDGET_WIDGETS_TEXTBOX_HPP
#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/system/key.hpp>
#include <cppurses/system/mouse_button.hpp>
#include <cppurses/widget/widgets/textbox_base.hpp>

#include <cstddef>
#include <cstdint>

namespace cppurses {

class Textbox : public Textbox_base {
   public:
    explicit Textbox(Glyph_string contents = "");

    void enable_scrollwheel(bool enable = true);
    void disable_scrollwheel(bool disable = true);
    void toggle_scrollwheel();
    bool does_scrollwheel() const;
    void set_wheel_speed(std::size_t lines);
    void set_wheel_speed_up(std::size_t lines);
    void set_wheel_speed_down(std::size_t lines);

   protected:
    bool key_press_event(Key key, char symbol) override;
    bool mouse_press_event(Mouse_button button,
                           Point global,
                           Point local,
                           std::uint8_t device_id) override;

   private:
    bool scroll_wheel_{true};
    std::size_t scroll_speed_up_{1};
    std::size_t scroll_speed_down_{1};
};

}  // namespace cppurses
#endif  // WIDGET_WIDGETS_TEXTBOX_HPP
