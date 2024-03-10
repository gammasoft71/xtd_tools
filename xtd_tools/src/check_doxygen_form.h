/// @file
/// @brief Contains check_doxygen_form class.
#pragma once
#include <xtd/xtd.forms>

/// @brief Represents the namespace that contains application objects.
namespace xtd_tools {
  /// @brief Represents the main form.
  class check_doxygen_form final : public xtd::forms::form {
  public:
    /// @brief Initializes a new instance of the color_converter_form form.
    check_doxygen_form();
    
  private:
    void on_check_click();
    void on_browse_click();
    void on_form_closed(const xtd::forms::form_closed_event_args& e) override;
    void on_path_text_changed();
    void update_colors() noexcept;
    xtd::forms::button browse_button;
    xtd::forms::text_box path_text_box;
    xtd::forms::button check_button;
    xtd::forms::text_box result_text_box;
    xtd::forms::progress_dialog progress_dialog;
  };
}
