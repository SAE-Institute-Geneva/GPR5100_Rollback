
file(GLOB_RECURSE shmup_src include/*.h src/*.cpp)
add_executable(shmup ${shmup_src})
target_link_libraries(shmup PRIVATE CoreLib)