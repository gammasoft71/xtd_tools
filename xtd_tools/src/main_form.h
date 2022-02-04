/// @file
/// @brief Contains main_form class.
#pragma once
#include <vector>
#include <memory>
#include <xtd/xtd.forms>
#include "color_converter_form.h"
#include "style_sheet_renderer_form.h"

/// @brief Represents the namespace that contains application objects.
namespace xtd_tools {
  /// @brief Represents the main form.
  class main_form final : public xtd::forms::form {
  public:
    /// @brief Initializes a new instance of the main_form form.
    main_form();

    /// @brief The main entry point for the application.
    static void main();
    
  private:
    template<typename tool_t>
    void creates_form() noexcept {
      childs.push_back(std::make_shared<tool_t>());
      childs[childs.size() - 1]->visible(true);
      childs[childs.size() - 1]->form_closed += {*this, &main_form::on_child_form_closed};
    }

    void on_child_form_closed(xtd::object& sender, const xtd::forms::form_closed_event_args& e);
    void on_help_about_menu_item_click(xtd::object& sender, const xtd::event_args& e);

    xtd::forms::list_box tools_list_box;
    std::vector<std::shared_ptr<xtd::forms::form>> childs;

    xtd::forms::menu_item tools_color_converter_menu_item {"&Color converter", {*this, &main_form::creates_form<color_converter_form>}};
    xtd::forms::menu_item tools_style_sheet_renderer_menu_item {"&Style sheet renderer", {*this, &main_form::creates_form<style_sheet_renderer_form>}};
    xtd::forms::menu_item tools_separator_menu_item {"-"};
    xtd::forms::menu_item tools_exit_menu_item {xtd::forms::system_texts::exit(), overload_<>(&xtd::forms::application::exit), xtd::forms::shortcut::alt_f4};
    xtd::forms::menu_item tools_menu_item {"&Tools", {tools_color_converter_menu_item, tools_style_sheet_renderer_menu_item, tools_separator_menu_item, tools_exit_menu_item}};

    xtd::forms::menu_item help_about_menu_item {xtd::forms::system_texts::about(), {*this, &main_form::on_help_about_menu_item_click}};
    xtd::forms::menu_item help_menu_item {xtd::forms::system_texts::help(), {help_about_menu_item}};

    xtd::forms::main_menu main_menu {tools_menu_item, help_menu_item};
  };
}
