
add_executable(calc_main main.cpp)
target_include_directories(calc_main PUBLIC ${PROJECT_BASE_DIR})

target_link_libraries(calc_main kita_alef)
target_link_libraries(calc_main kita_bet)
