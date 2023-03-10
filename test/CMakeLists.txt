add_executable(testing main.c)
target_link_libraries(testing blib)

target_compile_definitions(blib PUBLIC _BTESTING)

file(COPY words.txt DESTINATION ${CMAKE_BINARY_DIR})
