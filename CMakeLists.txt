cmake_minimum_required(VERSION 3.25)
project(markdown_to_pdf)

set(CMAKE_CXX_STANDARD 20)

# Manuelle Einbindung von libharu
include_directories(/usr/include/libharu)
link_directories(/usr/lib/x86_64-linux-gnu)

# Manuelle Einbindung von cmark
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# Hinzufügen des ausführbaren Programms
add_executable(markdown_to_pdf src/main.cpp)

# Verlinke die libharu- und cmark-Bibliotheken
target_link_libraries(markdown_to_pdf PRIVATE hpdf cmark)
