cmake_minimum_required(VERSION 3.3)

# Project
project(xtd_tools)
find_package(xtd REQUIRED)
add_sources(
  src/main_form.h
  src/main_form.cpp
  src/color_converter_form.h
  src/color_converter_form.cpp
  src/style_sheet_renderer_form.h
  src/style_sheet_renderer_form.cpp
)
target_type(GUI_APPLICATION)

# Install
install_component()
