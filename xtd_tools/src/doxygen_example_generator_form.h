/// @file
/// @brief Contains doxygen_example_generator_form class.
#pragma once
#include <xtd/xtd.forms>

/// @brief Represents the namespace that contains application objects.
namespace xtd_tools {
  /// @brief Represents the main form.
  class doxygen_example_generator_form final : public xtd::forms::form {
  public:
    /// @brief Initializes a new instance of the color_converter_form form.
    doxygen_example_generator_form();
    
  private:
    inline static const xtd::ustring base_examples_path = xtd::io::path::combine({xtd::environment::get_folder_path(xtd::environment::special_folder::xtd_install), "share", "xtd", "examples"});
    xtd::ustring generate_xtd_cmake_examples();
    xtd::ustring generate_xtd_core_examples();
    xtd::ustring generate_xtd_drawing_examples();
    xtd::ustring generate_xtd_forms_examples();
    xtd::ustring generate_xtd_tunit_examples();
    void on_generate_click();
    xtd::forms::button browse_button;
    xtd::forms::text_box path_text_box;
    xtd::forms::choice xtd_examples_choice;
    xtd::forms::button generate_button;
    xtd::forms::text_box result_text_box;
  };
}
