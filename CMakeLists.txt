cmake_minimum_required(VERSION 3.25)
project(App1)
project(App2)

add_compile_options(-Werror -Wextra)
add_executable(app1 app1.cpp)
add_executable(app2 app2.cpp)
target_link_libraries(app2 pthread)