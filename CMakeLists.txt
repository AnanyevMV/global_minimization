cmake_minimum_required(VERSION 3.8)
project(global_minimization VERSION 1.0.0 LANGUAGES C CXX)

# используется версия C++ 17
set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB SOURCES src/Methods/*/*.cpp src/Tools/*.cpp)

include_directories(include)
include_directories(tms-nets)
include_directories(tms-nets/irrpoly/include)
# Если у Вас операционная система Windows, раскомментируйте строчку ниже (для подключения Pthreads)
# include_directories(lib/include)

add_executable(global_minimization ${SOURCES})

# Если у Вас операционная система Windows, раскомментируйте строчку ниже (для подключения Pthreads)
# add_compile_definitions(HAVE_STRUCT_TIMESPEC)
# Строчка выше решает следующую проблему: https://stackoverflow.com/questions/33557506/timespec-redefinition-error
# Если у Вас операционная система Windows, раскомментируйте строчку ниже (для подключения Pthreads)
# find_library(PTHREAD_LIBRARY pthreadVC2 HINTS lib)
# Если у Вас операционная система Windows, раскомментируйте строчку ниже (для подключения Pthreads)
# target_link_libraries(global_minimization PUBLIC ${PTHREAD_LIBRARY})
