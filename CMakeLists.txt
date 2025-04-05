cmake_minimum_required(VERSION 3.25)
project(Projet_Cpp LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE sourceCode src/*.cpp src/*.h)

qt_standard_project_setup(
        PROJECT_NAME Projet_Cpp
        VERSION 1.0
        DESCRIPTION "Projet C++"
        LANGUAGES CXX

)

add_executable(Projet_Cpp ${sourceCode})

target_link_libraries(Projet_Cpp PRIVATE Qt6::Widgets)
