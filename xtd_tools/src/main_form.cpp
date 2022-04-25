#include "main_form.h"
#include "../properties/resources.h"

using namespace std;
using namespace xtd;
using namespace xtd::forms;
using namespace xtd_tools;

main_form::main_form() {
  text("xtd tools");
  client_size({300, 400});
  padding(10);
  menu(main_menu);
  
  tools_list_box.parent(*this);
  tools_list_box.dock(dock_style::fill);
  tools_list_box.items({"Color converter", "Release note generator", "Style sheet renderer", });
  
  tools_list_box.double_click += [&] {
    if (tools_list_box.selected_item().value() == "Color converter") creates_form<color_converter_form>();
    if (tools_list_box.selected_item().value() == "Release note generator") creates_form<release_note_generator_form>();
    if (tools_list_box.selected_item().value() == "Style sheet renderer") creates_form<style_sheet_renderer_form>();
  };
}

void main_form::main() {
  application::run(main_form());
}

void main_form::on_child_form_closed(object& sender, const form_closed_event_args& e) {
  for (auto it = childs.begin(); it != childs.end(); ++it)
    if (it->get() == &sender) {childs.erase(it); break;}
}

void main_form::on_help_about_menu_item_click(object& sender, const xtd::event_args& e) {
  about_box::show("The xtd_tools project is a collection of\ntools to help xtd development.", "xtd tools", properties::resources::xtd_tools(), "1.0", "1.0.0", "Copyright (c) 2022 Gammasoft.\nAll rights reserved.", "https://gammasoft71.wixsite.com/gammasoft", "Gammasoft");
}

void main_form::on_help_xtd_reference_guide_item_click(object& sender, const xtd::event_args& e) {
  diagnostics::process::start("https://codedocs.xyz/gammasoft71/xtd/index.html");
}
