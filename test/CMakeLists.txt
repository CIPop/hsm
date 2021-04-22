file (GLOB Sources "*.c")
add_executable(hsm_test ${Sources})
target_link_libraries(hsm_test hsm)

