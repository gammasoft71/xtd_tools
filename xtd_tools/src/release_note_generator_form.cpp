#include "release_note_generator_form.h"

using namespace xtd;
using namespace xtd::diagnostics;
using namespace xtd::forms;
using namespace xtd::io;
using namespace xtd_tools;

release_note_generator_form::release_note_generator_form() {
  text("Release note generator");
  client_size({1000, 600});
  controls().push_back_range({release_note_text_box, controls_panel});
  padding(10);
  
  controls_panel.controls().push_back_range({project_path_label, project_path_text_box, state_label, state_choice, milestone_label, milestone_text_box, generate_button});
  controls_panel.dock(dock_style::top);
  controls_panel.height(135);
  controls_panel.padding(10);
  
  project_path_label.text("Project path");
  project_path_label.auto_size(true);
  project_path_label.location({10, 12});
  
  project_path_text_box.text(path::combine(environment::get_folder_path(environment::special_folder::home), "Projects", "xtd"));
  project_path_text_box.location({90, 10});
  project_path_text_box.width(880);
  project_path_text_box.anchor(anchor_styles::left | anchor_styles::top |anchor_styles::right);
  
  state_label.text("State");
  state_label.auto_size(true);
  state_label.location({10, 42});
  
  state_choice.items().push_back_range({"closed", "open", "all"});
  state_choice.selected_index(0);
  state_choice.location({90, 40});
  state_choice.width(880);
  state_choice.anchor(anchor_styles::left | anchor_styles::top |anchor_styles::right);
  
  milestone_label.text("Milestone");
  milestone_label.auto_size(true);
  milestone_label.location({10, 72});
  
  milestone_text_box.text(environment::version().to_string());
  milestone_text_box.location({90, 70});
  milestone_text_box.width(880);
  milestone_text_box.anchor(anchor_styles::left | anchor_styles::top |anchor_styles::right);

  generate_button.click += [&]{
    progress_dialog dialog;
    dialog.text("Generate...");
    dialog.marquee(true);
    dialog.show_dialog(*this);
    application::do_events();
    release_note_text_box.text(generate_release_note(project_path_text_box.text(), state_choice.selected_item().value(), milestone_text_box.text()));
  };
  generate_button.dock(dock_style::bottom);
  generate_button.text("Generate");

  release_note_text_box.dock(dock_style::fill);
  release_note_text_box.multiline(true);
  release_note_text_box.read_only(true);
  release_note_text_box.word_wrap(false);
}

xtd::ustring release_note_generator_form::generate_release_note(const xtd::ustring& project_path, const xtd::ustring& state, const xtd::ustring& milestone) {
  auto repository = "https://github.com/" + get_repository(project_path);
  process generate_process;
  process_start_info psi;
  generate_process.start_info().use_shell_execute(false);
  generate_process.start_info().file_name("gh");
  
  ustring arguments = "issue list --limit 100000";
  if (state != "") arguments += ustring::format(" --state {}", state);
  if (milestone != "") arguments += ustring::format(" --milestone {}", milestone);
  generate_process.start_info().arguments(arguments);
  generate_process.start_info().working_directory(project_path);
  generate_process.start_info().redirect_standard_output(true);
  application::do_events();
  if (!generate_process.start()) {
    message_box::show(*this, "The generation process failed. Be sure you have installed Github CLI.", "Generation error", message_box_buttons::ok, message_box_icon::error);
    return "";
  }
  application::do_events();
  std::istream& standard_output = generate_process.standard_output();
  
  ustring result;
    stream_reader reader(standard_output);
  while (!reader.end_of_stream()) {
    auto line = reader.read_line();
    //xtd::diagnostics::debug::write_line(line);
    auto items = line.split({'\t'});
    if (items.size() < 4) continue;
    auto status = "todo"_s;
    if (items[1] == "CLOSED" && items[3].contains("enhancement"))
      status = "added"_s;
    else if (items[1] == "CLOSED" && !items[3].contains("enhancement"))
      status = "fixed"_s;
    result += ustring::format("* ![{0}]({1}/blob/master/docs/pictures/releases/status/{0}.png) {3} - [#{2}]({1}/issues/{2}){4}", status, repository, items[0], items[2], environment::new_line());
    application::do_events();
  }

  application::do_events();
  return result;
}

xtd::ustring release_note_generator_form::get_repository(const xtd::ustring& project_path) {
  process generate_process;
  process_start_info psi;
  generate_process.start_info().use_shell_execute(false);
  generate_process.start_info().file_name("gh");
  
  ustring arguments = "repo view";
  generate_process.start_info().arguments(arguments);
  generate_process.start_info().working_directory(project_path);
  generate_process.start_info().redirect_standard_output(true);
  application::do_events();
  if (!generate_process.start()) {
    message_box::show(*this, "The generation process failed. Be sure you have installed Github CLI.", "Generation error", message_box_buttons::ok, message_box_icon::error);
    return "";
  }
  application::do_events();
  std::istream& standard_output = generate_process.standard_output();
  
  stream_reader reader(standard_output);
  application::do_events();
  return reader.read_line().replace("name:\t", "");
}
