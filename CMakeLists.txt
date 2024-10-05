cmake_minimum_required(VERSION 3.25)
project(markdown_to_pdf)

set(CMAKE_CXX_STANDARD 20)

# Finde TinyXML2
find_package(tinyxml2 REQUIRED)

# Manuelle Einbindung von libharu
include_directories(/usr/include/libharu)
link_directories(/usr/lib/x86_64-linux-gnu)

# Hinzufügen des ausführbaren Programms
add_executable(markdown_to_pdf src/main.cpp src/header/main.hpp)

# Verlinke die TinyXML2- und libharu-Bibliotheken
target_link_libraries(markdown_to_pdf PRIVATE tinyxml2 hpdf)
