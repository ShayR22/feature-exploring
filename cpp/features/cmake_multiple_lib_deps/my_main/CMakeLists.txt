project(calc_main)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BASE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
    kita_bet
    pthread
)


install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${INSTALL_DIR}/bin/${PROJECT_NAME}
)
