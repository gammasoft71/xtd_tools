/// @file
/// @brief Contains release_note_generator_form class.
#pragma once
#include <xtd/xtd.forms>

/// @brief Represents the namespace that contains application objects.
namespace xtd_tools {
  /// @brief Represents the main form.
  class release_note_generator_form final : public xtd::forms::form {
  public:
    /// @brief Initializes a new instance of the release_note_generator_form form.
    release_note_generator_form();
    
  private:
    xtd::ustring generate_release_note(const xtd::ustring& project_path, const xtd::ustring& state, const xtd::ustring& milestone);
    xtd::ustring get_repository(const xtd::ustring& project_path);
    xtd::forms::label project_path_label;
    xtd::forms::text_box project_path_text_box;
    xtd::forms::label state_label;
    xtd::forms::choice state_choice;
    xtd::forms::label milestone_label;
    xtd::forms::text_box milestone_text_box;
    xtd::forms::panel controls_panel;
    xtd::forms::button generate_button;
    xtd::forms::text_box release_note_text_box;
  };
}
