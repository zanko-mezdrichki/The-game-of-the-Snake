cmake_minimum_required(VERSION 3.16)
project(TheGameOfTheSnake VERSION 0.1 LANGUAGES CXX)

include(CTest)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_SANITIZERS "Enable sanitizers in Debug builds" OFF)

# warning options
add_compile_options(
  -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion
  -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast
)

# sanitizers in Debug
if (ENABLE_SANITIZERS AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options($<$<CONFIG:Debug>:-fsanitize=address,undefined>)
  add_link_options($<$<CONFIG:Debug>:-fsanitize=address,undefined>)
endif()

# Cerca i .cpp in src/
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Dipendenza SFML
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

# --- Eseguibile principale ---
add_executable(gioco ${SRC_FILES})

# Include directories
target_include_directories(gioco PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link SFML
target_link_libraries(gioco PRIVATE sfml-graphics)

# Directory di output (dove finisce l’eseguibile)
set_target_properties(gioco PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copia gli asset dopo la compilazione
add_custom_command(TARGET gioco POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:gioco>/assets"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:gioco>/assets"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/immagini_e_font" "$<TARGET_FILE_DIR:gioco>/assets/immagini_e_font"
  COMMENT "Copying assets to runtime folder"
)

# --- Test ---
if (BUILD_TESTING)
  file(GLOB TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/*.cpp")
  foreach(tfile ${TEST_SOURCES})
    get_filename_component(tname ${tfile} NAME_WE)
    add_executable(${tname} ${tfile})
    target_include_directories(${tname} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${tname} PRIVATE sfml-graphics)
    set_target_properties(${tname} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests
    )
    add_test(NAME ${tname} COMMAND ${CMAKE_BINARY_DIR}/bin/tests/${tname})
  endforeach()
endif()
