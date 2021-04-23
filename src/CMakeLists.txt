file(GLOB Sources "*.c")

include_directories("../inc")

add_library(hfsm STATIC ${Sources})
