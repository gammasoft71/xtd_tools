#include "enumeration_unit_tests_generator_form.h"

using namespace std;
using namespace xtd;
using namespace xtd::diagnostics;
using namespace xtd::forms;
using namespace xtd::io;
using namespace xtd_tools;

enumeration_unit_tests_generator_form::enumeration_unit_tests_generator_form() {
  text("Enumeration unit tests generator");
  controls().push_back_range({enumeration_text_box, generate_button, result_text_box});
  client_size({800, 600});
  accept_button(generate_button);

  enumeration_text_box.bounds({10, 10, 660, 25});
  enumeration_text_box.anchor(anchor_styles::left |anchor_styles::top | anchor_styles::right);
  enumeration_text_box.placeholder_text("Enumeration name with namespace (example: xtd::net::sockets::address_family)");
  enumeration_text_box.text_changed += [&] {generate_button.enabled(!enumeration_text_box.text().empty());};

  generate_button.text("Generate");
  generate_button.bounds({690, 10, 100, 25});
  generate_button.anchor(anchor_styles::top | anchor_styles::right);
  generate_button.enabled(!enumeration_text_box.text().empty());
  generate_button.click += {*this, &enumeration_unit_tests_generator_form::on_generate_click};
  
  result_text_box.bounds({10, 45, 780, 545});
  result_text_box.anchor(anchor_styles::left | anchor_styles::top | anchor_styles::right | anchor_styles::bottom);
  result_text_box.accepts_return(true);
  result_text_box.accepts_tab(true);
  result_text_box.multiline(true);
  result_text_box.read_only(true);
  result_text_box.word_wrap(false);
  result_text_box.font({drawing::font_family::generic_monospace(), result_text_box.font().size()});
}

void enumeration_unit_tests_generator_form::on_generate_click() {
  result_text_box.clear();
  background_worker = make_unique<xtd::forms::background_worker>();
  background_worker->do_work += [&](object & sender, do_work_event_args & e) {
    begin_invoke([&] {
      progress_dialog = make_unique<xtd::forms::progress_dialog>();
      progress_dialog->text(ustring::format("Generate \"{}\" unit tests", as<ustring>(e.argument())));
      progress_dialog->message("Please wait...");
      progress_dialog->marquee(true);
      progress_dialog->show_sheet_dialog(*this);
    });
    auto last_separator_index = as<ustring>(e.argument()).find_last_of("::");
    auto enumeration = last_separator_index == as<ustring>(e.argument()).npos ? as<ustring>(e.argument()) : as<ustring>(e.argument()).substring(last_separator_index + 1);
    auto namespace_str = last_separator_index == as<ustring>(e.argument()).npos ?  "" : as<ustring>(e.argument()).remove(last_separator_index - 1);
    auto temp_project_path = path::combine(path::get_temp_path(), "xtd_enum_project");
    if (directory::exists(temp_project_path)) directory::remove(temp_project_path, true);
    directory::create_directory(temp_project_path);
    xtd::io::file::write_all_lines(path::combine(temp_project_path, "source.cpp"), get_source_file_text(enumeration, namespace_str));
    xtd::io::file::write_all_lines(path::combine(temp_project_path, "CMakeLists.txt"), get_cmakelist_file_text());
    auto process = xtd::diagnostics::process::start(process_start_info().file_name("xtdc").arguments(ustring::format("run {}", temp_project_path)).redirect_standard_output(true).use_shell_execute(false).create_no_window(true)).wait_for_exit();
    using_(stream_reader sr(process.standard_output())) {
      result_process_unit_tests = sr.read_to_end();
      if (result_process_unit_tests.contains("** BUILD SUCCEEDED **")) result_process_unit_tests = result_process_unit_tests.substring(result_process_unit_tests.last_index_of("** BUILD SUCCEEDED **") + 24);
      begin_invoke([&] {
        result_text_box.text(result_process_unit_tests);
      });
    }
  };
  background_worker->run_worker_completed += [&] {
    begin_invoke([&] {
      progress_dialog->hide();
      progress_dialog.reset();
      background_worker.reset();
      generate_button.enabled(!enumeration_text_box.text().empty());
    });
  };
  background_worker->run_worker_async(enumeration_text_box.text());
}
