/// @file
/// @brief Contains doxygen_example_generator_form class.
#pragma once
#include <xtd/xtd.forms>

/// @brief Represents the namespace that contains application objects.
namespace xtd_tools {
  /// @brief Represents the main form.
  class enumeration_unit_tests_generator_form final : public xtd::forms::form {
  public:
    /// @brief Initializes a new instance of the color_converter_form form.
    enumeration_unit_tests_generator_form();
    
  private:
    inline static const xtd::ustring base_examples_path = xtd::io::path::combine({xtd::environment::get_folder_path(xtd::environment::special_folder::xtd_install), "share", "xtd", "examples"});
    void on_generate_click();
    std::unique_ptr<xtd::forms::background_worker> background_worker;
    xtd::forms::text_box enumeration_text_box;
    xtd::forms::button generate_button;
    std::unique_ptr<xtd::forms::progress_dialog> progress_dialog;
    xtd::forms::text_box result_text_box;
    xtd::ustring result_process_unit_tests;
    static std::vector<xtd::ustring> get_source_file_text(const xtd::ustring& enumeration, const xtd::ustring& namespace_str) {
      return {
        "#include <xtd/xtd>",
        "",
        "using namespace xtd;",
        "",
        "class program {",
        "public:",
        "  static void main() {",
        xtd::ustring::format("    console::write_line(to_unit_test_string<{0}::{1}>(\"{0}\"));", namespace_str, enumeration),
        "  }",
        "",
        "private:",
        "  template<typename enum_t>",
        "  static ustring to_unit_test_string(const ustring& namespace_str) {",
        "    ustring result;",
        "    result += ustring::format(\"#include <{}/{}.h>\\n\", namespace_str.replace(\"::\", \"/\"), ustring::class_name<enum_t>());",
        "    result += \"#include <xtd/xtd.tunit>\\n\";",
        "    result += \"\\n\";",
        "    result += ustring::format(\"using namespace {};\\n\", namespace_str);",
        "    result += \"using namespace xtd::tunit;\\n\";",
        "    result += \"\\n\";",
        "    result += \"namespace xtd::tests {\\n\";",
        "    result += ustring::format(\"  class test_class_({}_tests) {{\\n\", ustring::class_name<enum_t>());",
        "    result += \"  public:\\n\";",
        "    auto first = true;",
        "    for (auto [key, value] : enum_object<>::get_entries<enum_t>()) {",
        "      if (!first) result += \"    \\n\";",
        "      result += ustring::format(\"    void test_method_({}) {{\\n\", key);",
        "      result += ustring::format(\"      assert::are_equal({}, enum_object<>::to_int32({}::{}), csf_);\\n\", as<int32>(key), ustring::class_name<enum_t>(), value);",
        "      result += ustring::format(\"      assert::are_equal(\\\"{}\\\", enum_object<>::to_string({}::{}), csf_);\\n\", key, ustring::class_name<enum_t>(), value);",
        "      result += ustring::format(\"      assert::are_equal({}::{}, enum_object<>::parse<{}>(\\\"{}\\\"), csf_);\\n\", ustring::class_name<enum_t>(), value, ustring::class_name<enum_t>(), key);",
        "      result += \"    }\\n\";",
        "      first = false;",
        "    }",
        "    result += \"  };\\n\";",
        "    result += \"}\\n\";",
        "",
        "    return result;",
        "  }",
        "};",
        "",
        "startup_(program);",
      };
    }
    static std::vector<xtd::ustring> get_cmakelist_file_text() {
      return {
        "cmake_minimum_required(VERSION 3.3)",
        "",
        "# Project",
        "project(xtd_enum_project)",
        "find_package(xtd REQUIRED)",
        "add_references(xtd.tunit xtd.forms)",
        "add_sources(",
        "  source.cpp",
        ")",
        "target_type(CONSOLE_APPLICATION)",
        "",
        "# Install",
        "install_component()",
      };
    }
  };
}
