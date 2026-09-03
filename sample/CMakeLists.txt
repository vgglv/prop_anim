file(GLOB_RECURSE SOURCES "src/*.*")

add_executable(prop_anim_sample ${SOURCES})

add_subdirectory(third_party)
target_link_libraries(prop_anim_sample raylib_static)