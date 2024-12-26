cmake_minimum_required(VERSION 3.10)
project(GtkCalculator)

# Encontra o pacote gtkmm
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

# Adiciona os diretórios de include do gtkmm
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

# Adiciona os arquivos fonte
add_executable(calc main.cpp calc.cpp)

# Linka as bibliotecas necessárias
target_link_libraries(calc ${GTKMM_LIBRARIES})
