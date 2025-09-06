cmake_minimum_required(VERSION 3.10)
project(HP_Folding)

# Ustawienie standardu C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ustawienie flag kompilatora
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

# Dodanie katalogów include
include_directories(Header)

# Utworzenie katalogu wyjściowego, jeśli nie istnieje
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Out)

# Znalezienie źródeł
file(GLOB_RECURSE SOURCES "Main/*.cpp")
file(GLOB_RECURSE HEADERS "Main/*.h" "Header/*.h")

# Dodanie executable
add_executable(hp_folding ${SOURCES} ${HEADERS})

# Ustawienie ścieżki wyjściowej dla plików wynikowych
set_target_properties(hp_folding PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Out
)

# Dodanie skryptów Pythonowych do post-build
find_package(Python3 COMPONENTS Interpreter)
if(Python3_FOUND)
    add_custom_command(TARGET hp_folding POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Generowanie wykresów i animacji..."
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Python/plot_energy.py
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Python/animate_folding.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Out
    )
endif()

# Instalacja
install(TARGETS hp_folding DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Python/plot_energy.py 
              ${CMAKE_CURRENT_SOURCE_DIR}/Python/animate_folding.py 
        DESTINATION share/hp_folding)
