set(lib dynstr)
project(${lib})
add_library(${lib} STATIC "dynstr.c")
target_include_directories(${lib} PUBLIC "include")
