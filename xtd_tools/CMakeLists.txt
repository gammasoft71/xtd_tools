cmake_minimum_required(VERSION 3.3)

# Project
project(xtd_tools)
find_package(xtd REQUIRED)
add_sources(
  src/main_form.h
  src/main_form.cpp
  src/color_converter_form.h
  src/color_converter_form.cpp
  src/doxygen_example_generator_form.h
  src/doxygen_example_generator_form.cpp
  src/release_note_generator_form.h
  src/release_note_generator_form.cpp
  src/sources_statistics_form.h
  src/sources_statistics_form.cpp
  src/style_sheet_renderer_form.h
  src/style_sheet_renderer_form.cpp
)
target_type(GUI_APPLICATION)

# Install
install_component()
