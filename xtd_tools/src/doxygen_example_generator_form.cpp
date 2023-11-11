#include "doxygen_example_generator_form.h"

using namespace xtd;
using namespace xtd::forms;
using namespace xtd::io;
using namespace xtd_tools;

namespace {
  ustring get_description(const xtd::ustring& file) {
    auto content = io::file::read_all_lines(file);
    if (content.size() < 2) return "";
    auto description = content[2];
    if (description.empty()) return "";
    static const std::regex rgx_md_link(R"(\[(.*?)\]\((.*?)\))", std::regex::optimize);
    xtd::ustring text = description;
    std::sregex_iterator iterator(description.begin(), description.end(), rgx_md_link), end{};
    for (auto it = iterator; it != end; ++it) {
      if (it->size() == 3) { // 3 matches: whole []() + sub [] + sub ()
        const xtd::ustring whole = it->str(0); // []()
        const xtd::ustring title = it->str(1); // [] contents
        text = text.replace(whole, title);
      }
    }
    
    return text;
  }
}

doxygen_example_generator_form::doxygen_example_generator_form() {
  text("Doxygen examples generator");
  controls().push_back_range({xtd_examples_choice, generate_button, result_text_box});
  client_size({800, 600});
  
  xtd_examples_choice.items().push_back_range({"xtd.core", "xtd.drawing", "xtd.forms", "xtd.tunit", "xtd.cmake"});
  xtd_examples_choice.bounds({10, 10, 660, 25});
  xtd_examples_choice.anchor(anchor_styles::left |anchor_styles::top | anchor_styles::right);
  xtd_examples_choice.selected_index(0);
  
  generate_button.text("Generate");
  generate_button.bounds({690, 10, 100, 25});
  generate_button.anchor(anchor_styles::top | anchor_styles::right);
  generate_button.click += {*this, &doxygen_example_generator_form::on_generate_click};
  
  result_text_box.bounds({10, 45, 780, 545});
  result_text_box.anchor(anchor_styles::left | anchor_styles::top | anchor_styles::right | anchor_styles::bottom);
  result_text_box.accepts_return(true);
  result_text_box.accepts_tab(true);
  result_text_box.multiline(true);
  result_text_box.read_only(true);
  result_text_box.word_wrap(false);
  result_text_box.font({drawing::font_family::generic_monospace(), result_text_box.font().size()});
}

void doxygen_example_generator_form::on_generate_click() {
  result_text_box.clear();
  switch(xtd_examples_choice.selected_index()) {
    case 0: result_text_box.text(generate_xtd_core_examples()); break;
    case 1: result_text_box.text(generate_xtd_drawing_examples()); break;
    case 2: result_text_box.text(generate_xtd_forms_examples()); break;
    case 3: result_text_box.text(generate_xtd_tunit_examples()); break;
    case 4: result_text_box.text(generate_xtd_cmake_examples()); break;
    default: break;
  }
}

xtd::ustring doxygen_example_generator_form::generate_xtd_cmake_examples() {
  ustring result;
  result += ustring::format("/// @file\n");
  result += ustring::format("/// @brief Contains xtd.cmake examples.\n");
  result += ustring::format("///\n");
  auto examples_dyrectory = path::combine(base_examples_path, "xtd.cmake.examples");
  auto type_directories = directory::get_directories(examples_dyrectory);
  sort(type_directories.begin(), type_directories.end());
  for (auto type_directory : type_directories) {
    auto directories = io::directory::get_directories(type_directory);
    sort(directories.begin(), directories.end());
    for (auto directory : directories) {
      auto files = io::directory::get_files(directory);
      for (auto file : files) {
        if (io::path::get_extension(file) == ".md") {
          result += ustring::format("/// @example {}.cpp\n", io::path::get_file_name(directory));
          result += ustring::format("/// {}\n", get_description(file));
          result += ustring::format("///\n");
          continue;
        }
      }
    }
  }
  return result;
}

xtd::ustring doxygen_example_generator_form::generate_xtd_core_examples() {
  ustring result;
  result += ustring::format("/// @file\n");
  result += ustring::format("/// @brief Contains xtd.core examples.\n");
  result += ustring::format("///\n");
  auto examples_dyrectory = path::combine(base_examples_path, "xtd.core.examples");
  auto type_directories = directory::get_directories(examples_dyrectory);
  sort(type_directories.begin(), type_directories.end());
  for (auto type_directory : type_directories) {
    auto directories = io::directory::get_directories(type_directory);
    sort(directories.begin(), directories.end());
    for (auto directory : directories) {
      auto files = io::directory::get_files(directory);
      for (auto file : files) {
        if (io::path::get_extension(file) == ".md") {
          result += ustring::format("/// @example {}.cpp\n", io::path::get_file_name(directory));
          result += ustring::format("/// {}\n", get_description(file));
          result += ustring::format("///\n");
          continue;
        }
      }
    }
  }
  return result;
}

xtd::ustring doxygen_example_generator_form::generate_xtd_drawing_examples() {
  ustring result;
  result += ustring::format("/// @file\n");
  result += ustring::format("/// @brief Contains xtd.drawing examples.\n");
  result += ustring::format("///\n");
  auto examples_dyrectory = path::combine(base_examples_path, "xtd.drawing.examples");
  auto type_directories = directory::get_directories(examples_dyrectory);
  sort(type_directories.begin(), type_directories.end());
  for (auto type_directory : type_directories) {
    auto directories = io::directory::get_directories(type_directory);
    sort(directories.begin(), directories.end());
    for (auto directory : directories) {
      auto files = io::directory::get_files(directory);
      for (auto file : files) {
        if (io::path::get_extension(file) == ".md") {
          result += ustring::format("/// @example {}.cpp\n", io::path::get_file_name(directory));
          result += ustring::format("/// {}\n", get_description(file));
          result += ustring::format("///\n");
          continue;
        }
      }
    }
  }
  return result;
}

xtd::ustring doxygen_example_generator_form::generate_xtd_forms_examples() {
  ustring result;
  result += ustring::format("/// @file\n");
  result += ustring::format("/// @brief Contains xtd.forms examples.\n");
  result += ustring::format("///\n");
  auto examples_dyrectory = path::combine(base_examples_path, "xtd.forms.examples");
  auto type_directories = directory::get_directories(examples_dyrectory);
  sort(type_directories.begin(), type_directories.end());
  for (auto type_directory : type_directories) {
    auto directories = io::directory::get_directories(type_directory);
    sort(directories.begin(), directories.end());
    for (auto directory : directories) {
      auto files = io::directory::get_files(directory);
      for (auto file : files) {
        if (io::path::get_extension(file) == ".md") {
          result += ustring::format("/// @example {}.cpp\n", io::path::get_file_name(directory));
          result += ustring::format("/// {}\n", get_description(file));
          result += ustring::format("/// @par Windows\n");
          result += ustring::format("/// @image html {}_w.png\n", io::path::get_file_name(directory));
          result += ustring::format("/// <br>\n");
          result += ustring::format("/// @image html {}_wd.png\n", io::path::get_file_name(directory));
          result += ustring::format("/// @par macOS\n");
          result += ustring::format("/// @image html {}_m.png\n", io::path::get_file_name(directory));
          result += ustring::format("/// <br>\n");
          result += ustring::format("/// @image html {}_md.png\n", io::path::get_file_name(directory));
          result += ustring::format("/// @par Gnome\n");
          result += ustring::format("/// @image html {}_g.png\n", io::path::get_file_name(directory));
          result += ustring::format("/// <br>\n");
          result += ustring::format("/// @image html {}_gd.png\n", io::path::get_file_name(directory));
          result += ustring::format("///\n");
          continue;
        }
      }
    }
  }
  return result;
}

xtd::ustring doxygen_example_generator_form::generate_xtd_tunit_examples() {
  auto result = ustring::empty_string;
  result += ustring::format("/// @file\n");
  result += ustring::format("/// @brief Contains xtd.tunit examples.\n");
  result += ustring::format("///\n");
  auto examples_dyrectory = path::combine(base_examples_path, "xtd.tunit.examples");
  auto type_directories = directory::get_directories(examples_dyrectory);
  sort(type_directories.begin(), type_directories.end());
  for (auto type_directory : type_directories) {
    auto directories = io::directory::get_directories(type_directory);
    sort(directories.begin(), directories.end());
    for (auto directory : directories) {
      auto files = io::directory::get_files(directory);
      for (auto file : files) {
        if (io::path::get_extension(file) == ".md") {
          result += ustring::format("/// @example {}.cpp\n", io::path::get_file_name(directory));
          result += ustring::format("/// {}\n", get_description(file));
          result += ustring::format("///\n");
          continue;
        }
      }
    }
  }
  return result;
}
