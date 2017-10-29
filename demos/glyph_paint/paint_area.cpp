#include "paint_area.hpp"

#include <cppurses/cppurses.hpp>
#include <signals/slot.hpp>

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <utility>

using namespace cppurses;

namespace demos {
namespace glyph_paint {

Paint_area::Paint_area() {
    enable_border(*this);
    disable_walls(this->border);
    disable_corners(this->border);
    this->border.east_enabled = true;
    this->focus_policy = Focus_policy::Strong;
}

void Paint_area::set_glyph(Glyph glyph) {
    current_glyph_ = std::move(glyph);
    glyph_changed(current_glyph_);
    if (erase_enabled_) {
        this->disable_erase();
        erase_disabled();
    }
}

void Paint_area::set_symbol(const Glyph& symbol) {
    if (erase_enabled_) {
        this->disable_erase();
        erase_disabled();
    }
    current_glyph_.set_symbol(symbol.str());
    glyph_changed(current_glyph_);
}

void Paint_area::set_foreground_color(Color c) {
    current_glyph_.brush().set_foreground(c);
    if (!erase_enabled_) {
        glyph_changed(current_glyph_);
    }
}

void Paint_area::set_background_color(Color c) {
    current_glyph_.brush().set_background(c);
    if (!erase_enabled_) {
        glyph_changed(current_glyph_);
    }
}

void Paint_area::set_attribute(Attribute attr) {
    current_glyph_.brush().add_attributes(attr);
    if (!erase_enabled_) {
        glyph_changed(current_glyph_);
    }
}

void Paint_area::remove_attribute(Attribute attr) {
    current_glyph_.brush().remove_attribute(attr);
    if (!erase_enabled_) {
        glyph_changed(current_glyph_);
    }
}

void Paint_area::enable_erase() {
    erase_enabled_ = true;
    // before_erase_ = current_glyph_;
    // current_glyph_ = " "; // std::exchange
    glyph_changed(" ");
}

void Paint_area::disable_erase() {
    erase_enabled_ = false;
    glyph_changed(current_glyph_);
    // this->set_glyph(before_erase_);
}

void Paint_area::enable_grid() {
    this->background_tile = Glyph{"┼", foreground(Color::Dark_gray)};
    this->update();
}

void Paint_area::disable_grid() {
    this->background_tile = " ";
    this->update();
}

void Paint_area::clear() {
    glyphs_painted_.clear();
    this->update();
}

Glyph Paint_area::glyph() const {
    return current_glyph_;
}

void Paint_area::toggle_clone() {
    clone_enabled_ = !clone_enabled_;
}

bool Paint_area::paint_event() {
    Painter p{this};
    for (const auto& gc_pair : glyphs_painted_) {
        if (gc_pair.first.x < this->width() &&
            gc_pair.first.y < this->height()) {
            p.put(gc_pair.second, gc_pair.first);
        }
    }
    return Widget::paint_event();
}

// bool Paint_area::resize_event(std::size_t new_width,
//                               std::size_t new_height,
//                               std::size_t old_width,
//                               std::size_t old_height) {
//     // Only expand the matrix, do not delete it
//     if (new_width > matrix.width() || new_height > matrix.height()) {
//         matrix.resize(new_width, new_height);
//     }
//     return Matrix_display::resize_event(new_width, new_height, old_width,
//                                         old_height);
// }

bool Paint_area::mouse_press_event(Mouse_button button,
                                   std::size_t global_x,
                                   std::size_t global_y,
                                   std::size_t local_x,
                                   std::size_t local_y,
                                   std::uint8_t device_id) {
    this->place_glyph(local_x, local_y);
    return Widget::mouse_press_event(button, global_x, global_y, local_x,
                                     local_y, device_id);
}

bool Paint_area::key_press_event(Key key, char symbol) {
    if (!this->cursor_visible()) {
        return Widget::key_press_event(key, symbol);
    }
    if (this->width() == 0 || this->height() == 0) {
        return Widget::key_press_event(key, symbol);
    }
    std::size_t new_x{this->cursor_x() + 1};
    std::size_t new_y{this->cursor_y() + 1};
    switch (key) {
        case Key::Arrow_right:
            if (new_x == this->width()) {
                new_x = 0;
            }
            this->move_cursor_x(new_x);
            break;
        case Key::Arrow_left:
            this->move_cursor_x(this->cursor_x() - 1);
            break;
        case Key::Arrow_down:
            if (new_y == this->height()) {
                new_y = 0;
            }
            this->move_cursor_y(new_y);
            break;
        case Key::Arrow_up:
            this->move_cursor_y(this->cursor_y() - 1);
            break;
        case Key::Enter:
            this->place_glyph(this->cursor_x(), this->cursor_y());
            break;
        default:
            if (!std::iscntrl(symbol)) {
                this->set_symbol(symbol);
                this->place_glyph(this->cursor_x(), this->cursor_y());
                this->update();
            }
            break;
    }
    return Widget::key_press_event(key, symbol);
}

void Paint_area::place_glyph(std::size_t x, std::size_t y) {
    if (clone_enabled_) {
        if (glyphs_painted_.count(Coordinates{x, y}) == 1) {
            this->set_glyph(glyphs_painted_[Coordinates{x, y}]);
            this->toggle_clone();
        }
    } else if (erase_enabled_) {
        this->remove_glyph(Coordinates{x, y});
    } else {
        glyphs_painted_[Coordinates{x, y}] = current_glyph_;
        this->update();
    }
}

void Paint_area::remove_glyph(Coordinates coords) {
    glyphs_painted_.erase(coords);
    this->update();
}

namespace slot {

sig::Slot<void(Glyph)> set_glyph(Paint_area& pa) {
    sig::Slot<void(Glyph)> slot{[&pa](Glyph g) { pa.set_glyph(std::move(g)); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void()> set_glyph(Paint_area& pa, const Glyph& glyph) {
    sig::Slot<void()> slot{[&pa, glyph] { pa.set_glyph(glyph); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void(Glyph)> set_symbol(Paint_area& pa) {
    sig::Slot<void(Glyph)> slot{
        [&pa](Glyph symbol) { pa.set_symbol(std::move(symbol)); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void()> set_symbol(Paint_area& pa, const Glyph& symbol) {
    sig::Slot<void()> slot{[&pa, symbol] { pa.set_symbol(symbol); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void(Color)> set_foreground_color(Paint_area& pa) {
    sig::Slot<void(Color)> slot{[&pa](Color c) { pa.set_foreground_color(c); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void()> set_foreground_color(Paint_area& pa, Color c) {
    sig::Slot<void()> slot{[&pa, c] { pa.set_foreground_color(c); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void(Color)> set_background_color(Paint_area& pa) {
    sig::Slot<void(Color)> slot{[&pa](Color c) { pa.set_background_color(c); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void()> set_background_color(Paint_area& pa, Color c) {
    sig::Slot<void()> slot{[&pa, c] { pa.set_background_color(c); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void(Attribute)> set_attribute(Paint_area& pa) {
    sig::Slot<void(Attribute)> slot{
        [&pa](Attribute attr) { pa.set_attribute(attr); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void()> set_attribute(Paint_area& pa, Attribute attr) {
    sig::Slot<void()> slot{[&pa, attr] { pa.set_attribute(attr); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void(Attribute)> remove_attribute(Paint_area& pa) {
    sig::Slot<void(Attribute)> slot{
        [&pa](Attribute attr) { pa.remove_attribute(attr); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void()> remove_attribute(Paint_area& pa, Attribute attr) {
    sig::Slot<void()> slot{[&pa, attr] { pa.remove_attribute(attr); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void()> toggle_clone(Paint_area& pa) {
    sig::Slot<void()> slot{[&pa] { pa.toggle_clone(); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void()> clear(Paint_area& pa) {
    sig::Slot<void()> slot{[&pa] { pa.clear(); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void()> enable_erase(Paint_area& pa) {
    sig::Slot<void()> slot{[&pa] { pa.enable_erase(); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void()> disable_erase(Paint_area& pa) {
    sig::Slot<void()> slot{[&pa] { pa.disable_erase(); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void()> enable_grid(Paint_area& pa) {
    sig::Slot<void()> slot{[&pa] { pa.enable_grid(); }};
    slot.track(pa.destroyed);
    return slot;
}

sig::Slot<void()> disable_grid(Paint_area& pa) {
    sig::Slot<void()> slot{[&pa] { pa.disable_grid(); }};
    slot.track(pa.destroyed);
    return slot;
}

}  // namespace slot
}  // namespace glyph_paint
}  // namespace demos