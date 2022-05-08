/// @file
/// @brief Contains style_sheet_renderer_form class.
#pragma once
#include <xtd/xtd.forms>

/// @brief Represents the namespace that contains application objects.
namespace xtd_tools {
  /// @brief Represents the style_sheet_renderer_form form.
  class style_sheet_renderer_form final : public xtd::forms::form {
  public:
    /// @brief Initializes a new instance of the style_sheet_renderer_form form.
    style_sheet_renderer_form();
    
  private:
    xtd::forms::text_box style_sheet_text_box;
    xtd::forms::button run_button;
    xtd::forms::panel style_heet_panel;
    xtd::forms::button style_sheet_rendered_button;
    xtd::forms::label text_label;
    xtd::forms::text_box text_text_box;
    xtd::forms::label size_label;
    xtd::forms::numeric_up_down width_numeric_up_down;
    xtd::forms::numeric_up_down height_numeric_up_down;
    xtd::forms::label back_color_label;
    xtd::forms::text_box back_color_text_box;
    xtd::forms::label fore_color_label;
    xtd::forms::text_box fore_color_text_box;
  };
}
