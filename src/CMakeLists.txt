add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} glad glfw)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME_DEBUG ${PROJECT_NAME}D)
