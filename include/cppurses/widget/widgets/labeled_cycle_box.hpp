#ifndef WIDGET_WIDGETS_LABELED_CYCLE_BOX_HPP
#define WIDGET_WIDGETS_LABELED_CYCLE_BOX_HPP
#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/widget/layouts/horizontal_layout.hpp>
#include <cppurses/widget/widgets/cycle_box.hpp>
#include <cppurses/widget/widgets/label.hpp>
#include <cppurses/widget/widgets/push_button.hpp>

#include <signals/signals.hpp>

#include <string>
#include <vector>

namespace cppurses {

class Labeled_cycle_box : public Horizontal_layout {
   public:
    explicit Labeled_cycle_box(Glyph_string title = "");

    void set_title(Glyph_string title);

    Label& label{this->make_child<Label>()};
    Cycle_box& cycle_box{this->make_child<Cycle_box>()};

   private:
    void resize_label();
};

}  // namespace cppurses

#endif  // WIDGET_WIDGETS_LABELED_CYCLE_BOX_HPP
