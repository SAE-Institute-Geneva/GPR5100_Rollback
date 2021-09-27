
file(GLOB main_SRC *.cpp)
foreach(main_file ${main_SRC})
get_filename_component(main_project_name ${main_file} NAME_WE )
add_executable(${main_project_name} ${main_file})
target_link_libraries(${main_project_name} PRIVATE GameLib)

set_target_properties (${main_project_name} PROPERTIES FOLDER Game/Main)
endforeach()
