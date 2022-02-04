#include "style_sheet_renderer_form.h"

using namespace xtd;
using namespace xtd::drawing;
using namespace xtd::forms;
using namespace xtd_tools;

style_sheet_renderer_form::style_sheet_renderer_form() {
  text("Style sheet renderer");
  client_size({1000, 400});
  minimum_client_size({800, 200});
  maximize_box(false);
  controls().push_back_range({style_sheet_text_box, run_button, style_heet_panel});
  
  run_button.location({10, 10});
  run_button.anchor(anchor_styles::top | anchor_styles::left);
  run_button.text("Run");
  run_button.image(forms::button_images::from_name("media-playback-start"));
  run_button.image_align(content_alignment::middle_left);
  run_button.click += [&] {
    style_sheet_rendered_button.style_sheet(style_sheet_text_box.text());
    style_sheet_rendered_button.invalidate();
    style_sheet_rendered_button.refresh();
  };

  
  style_sheet_text_box.location({10, 40});
  style_sheet_text_box.size({650, 355});
  style_sheet_text_box.anchor(anchor_styles::top | anchor_styles::left | anchor_styles::bottom);
  style_sheet_text_box.multiline(true);
  style_sheet_text_box.word_wrap(false);
  style_sheet_text_box.text(
    "button {\n"
    "  margin: 1px 0px 1px 0px;\n"
    "  border-style: outset;\n"
    "  border-color: red;\n"
    "  border-width: 1px;\n"
    "  border-radius: 5px 0px 5px 0px;\n"
    "  background-image: linear-gradient(rgba(255, 0, 0, 48), rgba(255, 0, 0, 96));\n"
    "  padding: 1px 1px 3px 1px;\n"
    "  color: red;\n"
    "}\n"
    "\n"
    "button:pressed {\n"
    "  border-style: inset;\n"
    "  background-image: linear-gradient(rgba(255, 0, 0, 64), rgba(255, 0, 0, 128));\n"
    "  color: #FF5555;\n"
    "  }\n"
  );

  
  style_heet_panel.location({670, 10});
  style_heet_panel.border_style(forms::border_style::solid);
  style_heet_panel.size({320, 385});
  style_heet_panel.anchor(anchor_styles::top | anchor_styles::left | anchor_styles::bottom | anchor_styles::right);
  style_heet_panel.controls().push_back_range({style_sheet_rendered_button, back_color_label, back_color_text_box, fore_color_label, fore_color_text_box, size_label, width_numeric_up_down, height_numeric_up_down, text_label, text_text_box});

  style_sheet_rendered_button.location({10, 10});
  style_sheet_rendered_button.text("Button 1");

  size_label.location({10, 235});
  size_label.auto_size(true);
  size_label.text("size (W x H)");
  size_label.anchor(anchor_styles::left | anchor_styles::bottom);
  
  width_numeric_up_down.location({100, 235});
  width_numeric_up_down.width(100);
  width_numeric_up_down.value(style_sheet_rendered_button.width());
  width_numeric_up_down.maximum(10000);
  width_numeric_up_down.anchor(anchor_styles::left | anchor_styles::bottom);
  width_numeric_up_down.value_changed += [&]{
    style_sheet_rendered_button.width(width_numeric_up_down.value());
  };
  
  height_numeric_up_down.location({210, 235});
  height_numeric_up_down.width(100);
  height_numeric_up_down.value(style_sheet_rendered_button.height());
  height_numeric_up_down.maximum(10000);
  height_numeric_up_down.anchor(anchor_styles::left | anchor_styles::bottom);
  height_numeric_up_down.value_changed += [&]{
    style_sheet_rendered_button.height(height_numeric_up_down.value());
  };

  text_label.location({10, 265});
  text_label.auto_size(true);
  text_label.text("Text");
  text_label.anchor(anchor_styles::left | anchor_styles::bottom);

  text_text_box.location({100, 265});
  text_text_box.width(150);
  text_text_box.text(style_sheet_rendered_button.text());
  text_text_box.anchor(anchor_styles::left | anchor_styles::bottom);
  text_text_box.text_changed += [&]{
    style_sheet_rendered_button.text(text_text_box.text());
  };

  back_color_label.location({10, 295});
  back_color_label.auto_size(true);
  back_color_label.text("Back color");
  back_color_label.anchor(anchor_styles::left | anchor_styles::bottom);

  back_color_text_box.location({100, 295});
  back_color_text_box.width(150);
  back_color_text_box.text("control");
  back_color_text_box.anchor(anchor_styles::left | anchor_styles::bottom);
  back_color_text_box.text_changed += [&] {
    style_heet_panel.back_color(drawing::color::parse(back_color_text_box.text()));
  };
  
  fore_color_label.location({10, 325});
  fore_color_label.auto_size(true);
  fore_color_label.text("Fore color");
  fore_color_label.anchor(anchor_styles::left | anchor_styles::bottom);

  fore_color_text_box.location({100, 325});
  fore_color_text_box.width(150);
  fore_color_text_box.text("control text");
  fore_color_text_box.anchor(anchor_styles::left | anchor_styles::bottom);
  fore_color_text_box.text_changed += [&] {
    style_heet_panel.fore_color(drawing::color::parse(fore_color_text_box.text()));
  };

  run_button.perform_click();
}
