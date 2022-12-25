/// @file
/// @brief Contains color_converter_form class.
#pragma once
#include <xtd/xtd.forms>

/// @brief Represents the namespace that contains application objects.
namespace xtd_tools {
  /// @brief Represents the main form.
  class sources_statistics_form final : public xtd::forms::form {
  public:
    /// @brief Initializes a new instance of the color_converter_form form.
    sources_statistics_form();
    
  private:
    bool is_cloc_process_exist();
    xtd::ustring analyse_path(const xtd::ustring& path, const xtd::ustring& output_format);
    void on_analyse_click();
    void on_browse_click();
    void on_form_closed(const xtd::forms::form_closed_event_args& e) override;
    void on_path_text_changed();
    void update_colors() noexcept;
    xtd::forms::button browse_button;
    xtd::forms::text_box path_text_box;
    xtd::forms::choice format_choice;
    xtd::forms::button analyse_button;
    xtd::forms::tab_control result_tab_control;
    xtd::forms::tab_page all_result_tab_page;
    xtd::forms::tab_page sources_result_tab_page;
    xtd::forms::tab_page tests_result_tab_page;
    xtd::forms::tab_page examples_result_tab_page;
    xtd::forms::text_box all_result_text_box;
    xtd::forms::text_box sources_result_text_box;
    xtd::forms::text_box tests_result_text_box;
    xtd::forms::text_box examples_result_text_box;
  };
}
