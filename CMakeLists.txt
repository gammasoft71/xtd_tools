cmake_minimum_required(VERSION 3.20)

# Solution
project(xtd_tools)
find_package(xtd REQUIRED)
add_projects(
  xtd_tools
)

# Install
install_package()
