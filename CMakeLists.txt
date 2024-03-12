cmake_minimum_required(VERSION 3.18..3.20)

project(MultiThreading) # Set your project name

# Find pthread package (optional, but good practice)
find_package(Threads REQUIRED)

# Define the executable and its source files
add_executable(MyApp main.cpp) # Replace with your source files

# Link pthread to your executable
target_link_libraries(MyApp ${CMAKE_THREAD_LIBS_INIT})

