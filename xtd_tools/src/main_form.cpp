#include "main_form.h"
#include "../properties/resources.h"

using namespace std;
using namespace xtd;
using namespace xtd::forms;
using namespace xtd::io;
using namespace xtd_tools;

main_form::main_form() {
  text("xtd tools");
  client_size({300, 400});
  icon(properties::resources::xtd_tools_ico());
  padding(10);
  menu(main_menu);
  
  tools_list_box.parent(*this);
  tools_list_box.dock(dock_style::fill);
  tools_list_box.items({"Check doxygen", "Color converter", "Count examples", "Doxygen examples generator", "Enumeration unit tests generator", "Release note generator", "Sources statistics", "Style sheet renderer", });
  tools_list_box.selected_index(0);
  
  tools_list_box.double_click += [&] {
    if (tools_list_box.selected_item().value() == "Check doxygen") creates_form<check_doxygen_form>();
    else if (tools_list_box.selected_item().value() == "Color converter") creates_form<color_converter_form>();
    else if (tools_list_box.selected_item().value() == "Count examples") on_count_examples_double_click();
    else if (tools_list_box.selected_item().value() == "Doxygen examples generator") creates_form<doxygen_example_generator_form>();
    else if (tools_list_box.selected_item().value() == "Enumeration unit tests generator") creates_form<enumeration_unit_tests_generator_form>();
    else if (tools_list_box.selected_item().value() == "Release note generator") creates_form<release_note_generator_form>();
    else if (tools_list_box.selected_item().value() == "Sources statistics") creates_form<sources_statistics_form>();
    else if (tools_list_box.selected_item().value() == "Style sheet renderer") creates_form<style_sheet_renderer_form>();
  };
}

void main_form::main() {
  application::run(main_form());
}

void main_form::on_check_doxygen_click() {
}

void main_form::on_child_form_closed(object& sender, const form_closed_event_args& e) {
  for (auto it = childs.begin(); it != childs.end(); ++it)
    if (it->get() == &sender) {childs.erase(it); break;}
}

void main_form::on_count_examples_double_click() {
  auto count = 0;
  auto base_directory = path::combine(environment::get_folder_path(environment::special_folder::xtd_install), "share", "xtd", "examples");
  auto examples_dyrectories = vector {path::combine(base_directory, "xtd.core.examples"), path::combine(base_directory, "xtd.drawing.examples"), path::combine(base_directory, "xtd.forms.examples"), path::combine(base_directory, "xtd.tunit.examples"), path::combine(base_directory, "xtd.cmake.examples")};
  for (auto examples_dyrectory : examples_dyrectories) {
    auto type_directories = io::directory::get_directories(examples_dyrectory);
    sort(type_directories.begin(), type_directories.end());
    for (auto type_directory : type_directories) {
      auto directories = io::directory::get_directories(type_directory);
      sort(directories.begin(), directories.end());
      for (auto directory : directories) {
        ++count;
      }
    }
  }
  message_box::show(*this, ustring::format("There are {} examples!", count), "Examples");
}


void main_form::on_help_about_menu_item_click(object& sender, const xtd::event_args& e) {
  about_box::show("The xtd_tools project is a collection of\ntools to help xtd development.", "xtd tools", properties::resources::xtd_tools(), "1.0", "1.0.0", "Copyright (c) 2022 Gammasoft.\nAll rights reserved.", "https://gammasoft71.wixsite.com/gammasoft", "Gammasoft");
}

void main_form::on_help_xtd_reference_guide_item_click(object& sender, const xtd::event_args& e) {
  diagnostics::process::start("https://codedocs.xyz/gammasoft71/xtd/index.html");
}
