# Find all sources, headers and ui files.
file(GLOB_RECURSE PS_SOURCES  ./*.cpp)
file(GLOB_RECURSE PS_HEADERS  ./*.hpp)
file(GLOB_RECURSE PS_UI_FILES ./*.ui)

# Generate UI headers.
qt5_wrap_ui(PS_UI_HEADERS ${PS_UI_FILES})

# Compile the executable.
add_executable(PaperSoccer ${PS_SOURCES} ${PS_UI_HEADERS})
target_link_libraries(PaperSoccer Qt5::Core Qt5::Widgets)

# Install the compiled binary.
install(TARGETS PaperSoccer RUNTIME DESTINATION bin)
