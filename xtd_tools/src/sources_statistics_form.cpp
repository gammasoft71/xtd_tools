#include "sources_statistics_form.h"
#include "../properties/settings.h"

using namespace xtd;
using namespace xtd::forms;
using namespace xtd::io;
using namespace xtd_tools;

sources_statistics_form::sources_statistics_form() {
  if (!is_cloc_process_exist())
    message_box::show("cloc is not present. Please install cloc.\nSee https://github.com/AlDanial/cloc for more information.");
  
  text("Sources statistics");
  controls().push_back_range({browse_button, path_text_box, format_choice, analyse_button, result_tab_control});
  client_size({800, 600});
  accept_button(analyse_button);

  browse_button.text("Browse");
  browse_button.bounds({10, 10, 100, 25});
  browse_button.click += {*this, &sources_statistics_form::on_browse_click};
  
  path_text_box.placeholder_text("xtd sources path");
  path_text_box.bounds({120, 10, 450, 25});
  path_text_box.anchor(anchor_styles::left | anchor_styles::top | anchor_styles::right);
  path_text_box.text(properties::settings().xtd_sources_path());
  path_text_box.text_changed += {*this, &sources_statistics_form::on_path_text_changed};
  
  format_choice.items().push_back_range({{"Standard", " "}, {"CSV", "--csv"}, {"Json", "--json"}, {"Markdown", "--md"}, {"XML", "--xml"}, {"YAML", "--yaml"}});
  format_choice.bounds({580, 10, 100, 25});
  format_choice.anchor(anchor_styles::top | anchor_styles::right);
  format_choice.selected_index(0);
  
  analyse_button.text("Analyse");
  analyse_button.bounds({690, 10, 100, 25});
  analyse_button.anchor(anchor_styles::top | anchor_styles::right);
  analyse_button.enabled(is_cloc_process_exist() && !path_text_box.text().empty() && io::directory::exists(path_text_box.text()));
  analyse_button.click += {*this, &sources_statistics_form::on_analyse_click};
  
  result_tab_control.bounds({10, 45, 780, 545});
  result_tab_control.anchor(anchor_styles::left | anchor_styles::top | anchor_styles::right | anchor_styles::bottom);
  result_tab_control.tab_pages().push_back_range({all_result_tab_page, sources_result_tab_page, tests_result_tab_page, examples_result_tab_page, tools_result_tab_page});
  
  all_result_tab_page.text("All");
  sources_result_tab_page.text("Sources");
  tests_result_tab_page.text("Tests");
  examples_result_tab_page.text("Examples");
  tools_result_tab_page.text("tools");

  all_result_text_box.parent(all_result_tab_page);
  all_result_text_box.accepts_return(true);
  all_result_text_box.accepts_tab(true);
  all_result_text_box.dock(dock_style::fill);
  all_result_text_box.multiline(true);
  all_result_text_box.read_only(true);
  all_result_text_box.font({drawing::font_family::generic_monospace(), sources_result_text_box.font().size()});
  
  sources_result_text_box.parent(sources_result_tab_page);
  sources_result_text_box.accepts_return(true);
  sources_result_text_box.accepts_tab(true);
  sources_result_text_box.dock(dock_style::fill);
  sources_result_text_box.multiline(true);
  sources_result_text_box.read_only(true);
  sources_result_text_box.font({drawing::font_family::generic_monospace(), sources_result_text_box.font().size()});

  tests_result_text_box.parent(tests_result_tab_page);
  tests_result_text_box.accepts_return(true);
  tests_result_text_box.accepts_tab(true);
  tests_result_text_box.dock(dock_style::fill);
  tests_result_text_box.multiline(true);
  tests_result_text_box.read_only(true);
  tests_result_text_box.font({drawing::font_family::generic_monospace(), tests_result_text_box.font().size()});
  
  examples_result_text_box.parent(examples_result_tab_page);
  examples_result_text_box.accepts_return(true);
  examples_result_text_box.accepts_tab(true);
  examples_result_text_box.dock(dock_style::fill);
  examples_result_text_box.multiline(true);
  examples_result_text_box.read_only(true);
  examples_result_text_box.font({drawing::font_family::generic_monospace(), examples_result_text_box.font().size()});

  tools_result_text_box.parent(tools_result_tab_page);
  tools_result_text_box.accepts_return(true);
  tools_result_text_box.accepts_tab(true);
  tools_result_text_box.dock(dock_style::fill);
  tools_result_text_box.multiline(true);
  tools_result_text_box.read_only(true);
  tools_result_text_box.font({drawing::font_family::generic_monospace(), tools_result_text_box.font().size()});
}

xtd::ustring sources_statistics_form::analyse_path(const ustring& path, const ustring& output_format) {
  auto psi = diagnostics::process_start_info {"cloc", ustring::format("--hide-rate --quiet --exclude-dir=build,reference_guide,resources {} {}", output_format, path)};
  psi.use_shell_execute(false);
  psi.create_no_window(true);
  psi.redirect_standard_output(true);
  auto process = diagnostics::process::start(psi);
  process.wait_for_exit();
  return stream_reader {process.standard_output()}.read_to_end();
}

bool sources_statistics_form::is_cloc_process_exist() {
  static auto found = false;
  if (found) return true;
  try {
    auto psi = diagnostics::process_start_info {"cloc", "--version"};
    psi.use_shell_execute(false);
    psi.create_no_window(true);
    diagnostics::process::start(psi);
    found = true;
  } catch(...) {
    return found;
  }
  return found;
}

void sources_statistics_form::on_analyse_click() {
  auto output_format = as<ustring>(format_choice.selected_item().tag());
  auto dialog = progress_dialog {};
  dialog.maximum(4);
  dialog.show(*this);
  dialog.value(1);
  application::do_events();
  if (directory::exists(path_text_box.text())) all_result_text_box.text(analyse_path(path_text_box.text(), output_format));
  dialog.value(2);
  application::do_events();
  if (directory::exists(path::combine(path_text_box.text(), "src"))) sources_result_text_box.text(analyse_path(path::combine(path_text_box.text(), "src"), output_format));
  dialog.value(3);
  application::do_events();
  if (directory::exists(path::combine(path_text_box.text(), "tests"))) tests_result_text_box.text(analyse_path(path::combine(path_text_box.text(), "tests"), output_format));
  dialog.value(4);
  application::do_events();
  if (directory::exists(path::combine(path_text_box.text(), "examples"))) examples_result_text_box.text(analyse_path(path::combine(path_text_box.text(), "examples"), output_format));
  application::do_events();
  dialog.value(5);
  application::do_events();
  if (directory::exists(path::combine(path_text_box.text(), "tools"))) tools_result_text_box.text(analyse_path(path::combine(path_text_box.text(), "tools"), output_format));
  dialog.hide();
}

void sources_statistics_form::on_browse_click() {
  auto dialog = folder_browser_dialog {};
  dialog.selected_path(path_text_box.text());
  if (dialog.show_sheet_dialog(*this) == forms::dialog_result::ok)
    path_text_box.text(dialog.selected_path());
}

void sources_statistics_form::on_form_closed(const xtd::forms::form_closed_event_args& e) {
  diagnostics::debug::write_line(ustring::format("path = {}", path_text_box.text()));
  properties::settings::default_settings().xtd_sources_path(path_text_box.text());
  properties::settings::default_settings().save();
  form::on_form_closed(e);
}

void sources_statistics_form::on_path_text_changed() {
  analyse_button.enabled(is_cloc_process_exist() && !path_text_box.text().empty() && directory::exists(path_text_box.text()));
}
