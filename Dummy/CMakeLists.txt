file(GLOB_RECURSE CURRENT_HEADER_FILES *.h *.hpp)
file(GLOB_RECURSE CURRENT_SOURCE_FILES *.cpp)
source_group("Header Files" FILES ${CURRENT_HEADER_FILES})
add_executable(Dummy
${CURRENT_SOURCE_FILES}
${CURRENT_HEADER_FILES}
)
target_link_libraries(Dummy Common)