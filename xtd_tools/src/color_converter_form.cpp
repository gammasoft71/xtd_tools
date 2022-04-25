#include "color_converter_form.h"
#include "../properties/resources.h"

using namespace xtd;
using namespace xtd::drawing;
using namespace xtd::forms;
using namespace xtd_tools;

color_converter_form::color_converter_form() {
  text("Color converter");
  client_size({470, 130});
  icon(properties::resources::xtd_tools_ico());
  maximize_box(false);
  minimum_client_size(client_size());
  maximum_client_size(client_size());
  controls().push_back_range({color_label, color_text_box, color_panel, color_value_label, darker_label, darker_numeric_up_down, darker_panel, darker_value_label, lighter_label, lighter_numeric_up_down, lighter_panel, lighter_value_label});
  
  color_text_box.text_changed += {*this, &color_converter_form::update_colors};
  darker_numeric_up_down.value_changed += {*this, &color_converter_form::update_colors};
  lighter_numeric_up_down.value_changed += {*this, &color_converter_form::update_colors};

  color_label.location({10, 11});
  color_label.auto_size(true);
  color_label.text("color");
  
  color_text_box.location({60, 11});
  color_text_box.width(150);
  color_text_box.text("control");
  
  color_panel.location({220, 10});
  color_panel.border_style(forms::border_style::inset);
  color_panel.size({150, 25});
  
  color_value_label.location({380, 11});
  color_value_label.auto_size(true);
  
  darker_label.location({10, 51});
  darker_label.auto_size(true);
  darker_label.text("dark");
  
  darker_numeric_up_down.location({60, 51});
  darker_numeric_up_down.width(150);
  darker_numeric_up_down.decimal_place(2);
  darker_numeric_up_down.increment(0.01);
  darker_numeric_up_down.set_range(.0, 1.0);
  darker_numeric_up_down.value(1.0 / 3);

  darker_panel.location({220, 50});
  darker_panel.border_style(forms::border_style::inset);
  darker_panel.size({150, 25});
  
  darker_value_label.location({380, 51});
  darker_value_label.auto_size(true);
  
  lighter_label.location({10, 91});
  lighter_label.auto_size(true);
  lighter_label.text("light");
  
  lighter_numeric_up_down.location({60, 91});
  lighter_numeric_up_down.width(150);
  lighter_numeric_up_down.decimal_place(2);
  lighter_numeric_up_down.increment(0.01);
  lighter_numeric_up_down.set_range(.0, 1.0);
  lighter_numeric_up_down.value(1.0 / 3);

  lighter_panel.location({220, 90});
  lighter_panel.border_style(forms::border_style::inset);
  lighter_panel.size({150, 25});
  
  lighter_value_label.location({380, 91});
  lighter_value_label.auto_size(true);
}

void color_converter_form::update_colors() noexcept {
  auto color = drawing::color::parse(color_text_box.text());
  color_panel.back_color(color);
  color_value_label.text(ustring::format("0x{:X8}", color.to_argb()));
  
  auto dark_color = drawing::color::dark(color, darker_numeric_up_down.value());
  darker_panel.back_color(dark_color);
  darker_value_label.text(ustring::format("0x{:X8}", dark_color.to_argb()));
  
  auto light_color = drawing::color::light(color, lighter_numeric_up_down.value());
  lighter_panel.back_color(light_color);
  lighter_value_label.text(ustring::format("0x{:X8}", light_color.to_argb()));
}
