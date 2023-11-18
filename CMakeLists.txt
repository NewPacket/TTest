add_library(ttest INTERFACE)
set(ttest_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(ttest PROPERTIES FOLDER "Shared")

target_sources(ttest PRIVATE ttest.h
                             ttest_internal.h
)

target_include_directories (ttest INTERFACE ${ttest_ROOT_DIR})