/// @file
/// @brief Contains form1 class.
#pragma once
#include <xtd/xtd.forms>

/// @brief Represents the namespace that contains application objects.
namespace xtd_tools {
  /// @brief Represents the main form.
  class style_sheet_renderer : public xtd::forms::form {
  public:
    /// @brief Initializes a new instance of the form1 class.
    style_sheet_renderer();
    
  private:
    xtd::forms::text_box style_sheet_text_box;
    xtd::forms::button apply_button;
    xtd::forms::panel style_heet_panel;
    xtd::forms::button style_sheet_rendered_button;
    xtd::forms::label back_color_label;
    xtd::forms::text_box back_color_text_box;
    xtd::forms::label fore_color_label;
    xtd::forms::text_box fore_color_text_box;
  };
}
