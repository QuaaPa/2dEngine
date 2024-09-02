cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME OpenGL)
project(${PROJECT_NAME})

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.hpp" "src/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)


set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
add_subdirectory(${EXTERNAL_DIR}/glfw)
add_subdirectory(${EXTERNAL_DIR}/glad)
add_subdirectory(${EXTERNAL_DIR}/stb_image)
add_subdirectory(${EXTERNAL_DIR}/imgui)

include_directories(${EXTERNAL_DIR})


target_link_libraries(${PROJECT_NAME} glfw glad stb_image imgui)

# include_directories(external)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)
