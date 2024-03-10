#include "check_doxygen_form.h"
#include "../properties/settings.h"
#include <xtd/threading/thread>
#include <xtd/threading/thread_pool>

using namespace xtd;
using namespace xtd::forms;
using namespace xtd::io;
using namespace xtd::threading;
using namespace xtd_tools;

class check_doxygen_keywords {
public:
  check_doxygen_keywords(const xtd::ustring& sources_path) : sources_path_ {sources_path} {
    if (!xtd::io::directory::exists(sources_path)) throw xtd::io::directory_not_found_exception {csf_};
  }
  
  int checked_files() const noexcept {return file_counter_;}
  
  const std::vector<xtd::ustring>& error_messages() const noexcept {return error_messages_;}
  
  bool stop_at_first_error() const noexcept {return stop_at_first_error_;}
  void stop_at_first_error(bool value) noexcept {stop_at_first_error_ = value;}
  
  bool check() {
    reset();
    return read_directories(sources_path_) && error_messages_.size() == 0;
  }
  
private:
  bool check_pair_brace(const xtd::ustring& file, const xtd::ustring& line) {
    if (line == "/// @}") --brace_counter_;
    if (brace_counter_ < 0) {
      error_messages_.push_back(xtd::ustring::format("Error brace counter < 0 => {}:{}", file, line_counter_));
      return false;
    }
    if (line == "/// @{") ++brace_counter_;
    return true;
  }
  
  bool check_pair_code(const xtd::ustring& file, const xtd::ustring& line) {
    if (line.contains("@code") && line.contains("@endcode")) return true;
    if (line == "/// @endcode") --code_counter_;
    if (code_counter_ < 0) {
      error_messages_.push_back(xtd::ustring::format("Error code counter < 0 => {}:{}", file, line_counter_));
      return false;
    }
    if (line == "/// @code") ++code_counter_;
    if (code_counter_ > 1) {
      error_messages_.push_back(xtd::ustring::format("Error code counter > 1 => {}:{}", file, line_counter_));
      return false;
    }
    return true;
  }
  
  bool check_pair_cond(const xtd::ustring& file, const xtd::ustring& line) {
    if (line.contains("@cond") && line.contains("@endcond")) return true;
    if (line == "/// @endcond") --cond_counter_;
    if (cond_counter_ < 0) {
      error_messages_.push_back(xtd::ustring::format("Error cond counter < 0 => {}:{}", file, line_counter_));
      return false;
    }
    if (line == "/// @cond") ++cond_counter_;
    if (cond_counter_ > 1) {
      error_messages_.push_back(xtd::ustring::format("Error cond counter > 1 => {}:{}", file, line_counter_));
      return false;
    }
    return true;
  }
  
  bool check_pair_verbatim(const xtd::ustring& file, const xtd::ustring& line) {
    if (line.contains("@verbatim") && line.contains("@endverbatim")) return true;
    if (line == "/// @endverbatim") --verbatim_counter_;
    if (verbatim_counter_ < 0) {
      error_messages_.push_back(xtd::ustring::format("Error verbatim counter < 0 => {}:{}", file, line_counter_));
      return false;
    }
    if (line == "/// @verbatim") ++verbatim_counter_;
    if (verbatim_counter_ > 1) {
      error_messages_.push_back(xtd::ustring::format("Error verbatim counter > 1 => {}:{}", file, line_counter_));
      return false;
    }
    return true;
  }
  
  bool is_valid_comment(const xtd::ustring& file, const xtd::ustring& line) {
    if (!line.starts_with("///@") && !line.starts_with("///  @") && !line.starts_with("///   @")) return true;
    error_messages_.push_back(xtd::ustring::format("Not a valid format (\\\ @) => {}:{}", file, line_counter_));
  }
  
  bool is_valid_keyword(const xtd::ustring& file, const xtd::ustring& line) {
    if (!line.starts_with("/// @")) return true;
    
    static auto keywords = std::set<xtd::ustring> {"@name ", "@{", "@}", "@cond", "@copyright ", "@endcond", "@code", "@endcode", "@file", "@verbatim", "@endverbatim", "@brief ", "@deprecated ", "@example ", "@exception ", "@image ", "@include ", "@ingroup ", "@note ", "@param ", "@par", "@remarks ", "@return ", "@see ", "@todo ", "@tparam ", "@warning "};
    for (auto keyword : keywords)
      if (line.contains(keyword)) return true;
    error_messages_.push_back(xtd::ustring::format("Not a valid keyword => {}:{}", file, line_counter_));
    return false;
  }
  
  bool read_directories(const xtd::ustring& path) {
    auto files = xtd::io::directory::get_files(path);
    std::sort(files.begin(), files.end());
    for (auto file : files) {
      if (xtd::io::path::get_extension(file) == ".h") {
        ++file_counter_;
        line_counter_ = 0;
        if (!read_file(file)) return false;
      }
    }
    
    auto directories = xtd::io::directory::get_directories(path);
    std::sort(directories.begin(), directories.end());
    for (auto directory : directories)
      if (!read_directories(directory)) return false;
    return true;
  }
  
  bool read_file(const xtd::ustring& file) {
    for (auto line : file::read_all_lines(file)) {
      ++line_counter_;
      line = line.trim();
      if (!is_valid_comment(file, line) && stop_at_first_error_) return false;
      if (!is_valid_keyword(file, line) && stop_at_first_error_) return false;
      if (!check_pair_brace(file, line) && stop_at_first_error_) return false;
      if (!check_pair_code(file, line) && stop_at_first_error_) return false;
      if (!check_pair_cond(file, line) && stop_at_first_error_) return false;
      if (!check_pair_verbatim(file, line) && stop_at_first_error_) return false;
    }
    if (brace_counter_ != 0) {
      error_messages_.push_back(xtd::ustring::format("Error brace counter {} 0 => {}:{}", brace_counter_ < 0 ? "<" : ">", file, line_counter_));
      if (stop_at_first_error_) return false;
    }
    if (code_counter_ != 0) {
      error_messages_.push_back(xtd::ustring::format("Error code counter {} 0 => {}:{}", code_counter_ < 0 ? "<" : ">", file, line_counter_));
      if (stop_at_first_error_) return false;
    }
    if (cond_counter_ != 0) {
      error_messages_.push_back(xtd::ustring::format("Error cond counter {} 0 => {}:{}", cond_counter_ < 0 ? "<" : ">", file, line_counter_));
      if (stop_at_first_error_) return false;
    }
    if (verbatim_counter_ != 0) {
      error_messages_.push_back(xtd::ustring::format("Error verbatim counter {} 0 => {}:{}", verbatim_counter_ < 0 ? "<" : ">", file, line_counter_));
      if (stop_at_first_error_) return false;
    }
    return true;
  }
  
  void reset() noexcept {
    brace_counter_ = 0;
    code_counter_ = 0;
    cond_counter_ = 0;
    file_counter_ = 0;
    line_counter_ = 0;
    verbatim_counter_ = 0;
    error_messages_.clear();
  }
  
  bool stop_at_first_error_ = false;
  int brace_counter_ = 0;
  int code_counter_ = 0;
  int cond_counter_ = 0;
  int file_counter_ = 0;
  int line_counter_ = 0;
  int verbatim_counter_ = 0;
  xtd::ustring sources_path_;
  std::vector<xtd::ustring> error_messages_;
};

check_doxygen_form::check_doxygen_form() {
  text("Check doxygen");
  controls().push_back_range({browse_button, path_text_box, check_button, result_text_box});
  client_size({800, 600});
  accept_button(check_button);

  browse_button.text("Browse");
  browse_button.bounds({10, 10, 100, 25});
  browse_button.click += {*this, &check_doxygen_form::on_browse_click};
  
  path_text_box.placeholder_text("xtd sources path");
  path_text_box.bounds({120, 10, 560, 25});
  path_text_box.anchor(anchor_styles::left | anchor_styles::top | anchor_styles::right);
  path_text_box.text(properties::settings().xtd_sources_path());
  path_text_box.text_changed += {*this, &check_doxygen_form::on_path_text_changed};
  
  check_button.text("Check");
  check_button.bounds({690, 10, 100, 25});
  check_button.anchor(anchor_styles::top | anchor_styles::right);
  check_button.enabled(!path_text_box.text().empty() && io::directory::exists(path_text_box.text()));
  check_button.click += {*this, &check_doxygen_form::on_check_click};

  result_text_box.bounds({10, 45, 780, 545});
  result_text_box.anchor(anchor_styles::left | anchor_styles::top | anchor_styles::right | anchor_styles::bottom);
  result_text_box.accepts_return(true);
  result_text_box.accepts_tab(true);
  result_text_box.multiline(true);
  result_text_box.read_only(true);
  result_text_box.font({drawing::font_family::generic_monospace(), result_text_box.font().size()});
  
  progress_dialog.marquee(true);
  progress_dialog.text("checking...");
}

void check_doxygen_form::on_check_click() {
  check_button.enabled(false);
  progress_dialog.show(*this);
  thread_pool::queue_user_work_item([&, path = path_text_box.text()] {
    auto test = std::make_shared<check_doxygen_keywords>(path);
    auto result = test->check();
    begin_invoke([&, invoke_result = result, invoke_test = test] {
      if (invoke_result) result_text_box.text(ustring::format("{} header files are checked with success", invoke_test->checked_files()));
      else result_text_box.text(ustring::join("\n", invoke_test->error_messages()));
      progress_dialog.hide();
      check_button.enabled(true);
    });
  });
}

void check_doxygen_form::on_browse_click() {
  auto dialog = folder_browser_dialog {};
  dialog.selected_path(path_text_box.text());
  if (dialog.show_sheet_dialog(*this) == forms::dialog_result::ok)
    path_text_box.text(dialog.selected_path());
}

void check_doxygen_form::on_form_closed(const xtd::forms::form_closed_event_args& e) {
  diagnostics::debug::write_line(ustring::format("path = {}", path_text_box.text()));
  properties::settings::default_settings().xtd_sources_path(path_text_box.text());
  properties::settings::default_settings().save();
  form::on_form_closed(e);
}

void check_doxygen_form::on_path_text_changed() {
  check_button.enabled(!path_text_box.text().empty() && directory::exists(path_text_box.text()));
}
