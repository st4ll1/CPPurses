#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/painter/painter.hpp>
#include <cppurses/system/mouse_button.hpp>
#include <cppurses/widget/widgets/push_button.hpp>

#include <utility>

namespace cppurses {

Push_button::Push_button(Glyph_string name) : Label{std::move(name)} {
    this->height_policy.type(Size_policy::Preferred);
    this->set_alignment(Alignment::Center);
}

bool Push_button::mouse_press_event(Mouse_button button,
                                    Point global,
                                    Point local,
                                    std::uint8_t device_id) {
    if (button == Mouse_button::Left) {
        clicked();
    }
    return Widget::mouse_press_event(button, global, local, device_id);
}

namespace slot {

sig::Slot<void()> click(Push_button& pb) {
    sig::Slot<void()> slot{[&pb] { pb.clicked(); }};
    slot.track(pb.destroyed);
    return slot;
}

}  // namespace slot

}  // namespace cppurses
