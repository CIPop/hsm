include_directories("../inc")

add_library(hfsm_hardcoded STATIC hfsm_common_events.c hfsm_hardcoded.c)
add_library(hfsm_stack STATIC hfsm_common_events.c hfsm_stack.c)
