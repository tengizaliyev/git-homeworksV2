# CMakeList.txt : CMake project for ClientManager, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(ClientManager)

add_executable(ClientManager main.cpp ClientManager.cpp)

# Укажи путь к libpqxx
add_subdirectory("C:/Users/ASUS ROG/OneDrive/Desktop/CppTasks/libpqxx-master/libpqxx" libpqxx-build)

# Включи C++17
target_compile_features(ClientManager PRIVATE cxx_std_17)

# Слинкуй с libpqxx
target_link_libraries(ClientManager PRIVATE pqxx)