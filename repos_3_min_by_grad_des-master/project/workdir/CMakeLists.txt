cmake_minimum_required(VERSION 2.8.9)

project(min_by_grad_des)

include_directories(
  "include"
  "include/Methods"
  "include/Methods/Adadelta"
  "include/Methods/Adagrad"
  "include/Methods/Adam"
  "include/Methods/AdaMax"
  "include/Methods/AMSGrad"
  "include/Methods/Nadam"
  "include/Methods/RmsProp"
  "include/Tools"
)

file(GLOB SOURCES "test/*.cpp" "src/*.cpp" "src/Methods/*.cpp" "src/Tools/*.cpp")


add_executable(test ${SOURCES})
