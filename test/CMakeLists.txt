include_directories("../inc", "./")

add_executable(test_queue test_queue.c)
add_test(NAME test_queue COMMAND test_queue)

add_executable(test_hfsm_stack test_hfsm_stack.c)
target_link_libraries(test_hfsm_stack hfsm_stack)
add_test(NAME test_hfsm_stack COMMAND test_hfsm_stack)

add_executable(test_hfsm_hardcoded test_hfsm_hardcoded.c)
target_link_libraries(test_hfsm_hardcoded hfsm_hardcoded)
add_test(NAME test_hfsm_hardcoded COMMAND test_hfsm_hardcoded)
