/// @file
/// @brief Contains color_converter_form class.
#pragma once
#include <xtd/xtd.forms>

/// @brief Represents the namespace that contains application objects.
namespace xtd_tools {
  /// @brief Represents the main form.
  class color_converter_form final : public xtd::forms::form {
  public:
    /// @brief Initializes a new instance of the color_converter_form form.
    color_converter_form();
    
  private:
    void update_colors() noexcept;
    xtd::forms::label color_label;
    xtd::forms::text_box color_text_box;
    xtd::forms::panel color_panel;
    xtd::forms::label color_value_label;

    xtd::forms::label darker_label;
    xtd::forms::numeric_up_down darker_numeric_up_down;
    xtd::forms::panel darker_panel;
    xtd::forms::label darker_value_label;

    xtd::forms::label lighter_label;
    xtd::forms::numeric_up_down lighter_numeric_up_down;
    xtd::forms::panel lighter_panel;
    xtd::forms::label lighter_value_label;
  };
}
