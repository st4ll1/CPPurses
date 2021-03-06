cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(libcppurses LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(libs/Signals)
include(GNUInstallDirs)
enable_testing()

set(SYSTEM_SOURCES
	"src/system/child_event.cpp"
    "src/system/clear_screen_event.cpp"
    "src/system/close_event.cpp"
    "src/system/deferred_delete_event.cpp"
    "src/system/disable_event.cpp"
    "src/system/enable_event.cpp"
    "src/system/event.cpp"
    "src/system/event_handler.cpp"
    "src/system/event_handler_slots.cpp"
    "src/system/event_invoker.cpp"
	"src/system/event_loop.cpp"
    "src/system/event_queue.cpp"
    "src/system/focus.cpp"
    "src/system/focus_event.cpp"
    "src/system/hide_event.cpp"
    "src/system/key.cpp"
    "src/system/key_event.cpp"
    "src/system/mouse_event.cpp"
	"src/system/move_event.cpp"
    "src/system/ncurses_event_listener.cpp"
    "src/system/on_tree_event.cpp"
    "src/system/paint_event.cpp"
	"src/system/resize_event.cpp"
    "src/system/show_event.cpp"
	"src/system/system.cpp"
    "src/system/shortcuts.cpp"
    )

set(PAINTER_SOURCES
    "src/painter/ncurses_paint_engine.cpp"
	"src/painter/painter.cpp"
	"src/painter/palette.cpp"
	"src/painter/glyph.cpp"
	"src/painter/brush.cpp"
	"src/painter/paint_buffer.cpp"
    "src/painter/glyph_matrix.cpp"
    "src/painter/glyph_string.cpp"
    )

set(WIDGET_SOURCES
    "src/widget/widget.cpp"
    "src/widget/widget_slots.cpp"
    "src/widget/widget_stack.cpp"
    "src/widget/widget_stack_menu.cpp"
    "src/widget/glyph_select_stack.cpp"
    "src/widget/glyph_select.cpp"
    "src/widget/cycle_stack.cpp"
    "src/widget/label.cpp"
	"src/widget/layout.cpp"
    "src/widget/line_edit.cpp"
    "src/widget/log.cpp"
    "src/widget/confirm_button.cpp"
    "src/widget/labeled_cycle_box.cpp"
	"src/widget/horizontal_layout.cpp"
    "src/widget/matrix_display.cpp"
    "src/widget/point.cpp"
	"src/widget/border.cpp"
    "src/widget/cycle_box.cpp"
    "src/widget/vertical_layout.cpp"
    "src/widget/status_bar.cpp"
    "src/widget/textbox.cpp"
    "src/widget/textbox_slots.cpp"
    "src/widget/slider.cpp"
    "src/widget/textbox_base.cpp"
    "src/widget/textbox_base_slots.cpp"
    "src/widget/push_button.cpp"
    "src/widget/horizontal_scrollbar.cpp"
    "src/widget/vertical_scrollbar.cpp"
    "src/widget/checkbox.cpp"
    "src/widget/titlebar.cpp"
    "src/widget/text_display.cpp"
    "src/widget/text_display_slots.cpp"
    "src/widget/color_select.cpp"
    "src/widget/menu.cpp"
    "src/widget/size_policy.cpp"
    "src/widget/blank_width.cpp"
    "src/widget/blank_height.cpp"
    )

set(TEST_SOURCES
    "test/system/system_test.cpp"
	"test/system/object_test.cpp"
	"test/system/event_loop_test.cpp"
	"test/system/event_test.cpp"
	"test/system/abstract_event_dispatcher_test.cpp"
	"test/system/thread_data_test.cpp"
	"test/system/posted_event_queue_test.cpp"
	"test/system/posted_event_test.cpp"
	"test/system/ncurses_event_dispatcher_test.cpp"

	"test/widget/widget_test.cpp"

	"test/painter/glyph_test.cpp"
	"test/painter/glyph_string_test.cpp"
	"test/painter/brush_test.cpp"
	"test/painter/palette_test.cpp"
	"test/painter/glyph_matrix_test.cpp"
    )

set(CHESS_DEMO_SOURCES
    "demos/chess/src/chessboard_widget.cpp"
    "demos/chess/src/position.cpp"
    "demos/chess/src/chess_engine.cpp"
    "demos/chess/src/invalid_position.cpp"
    "demos/chess/src/rules.cpp"
    "demos/chess/src/no_rules.cpp"
    "demos/chess/src/standard_rules.cpp"
    "demos/chess/src/state.cpp"
    "demos/chess/src/chessboard_borders.cpp"
    "demos/chess/src/chess_ui.cpp"
    "demos/chess/src/side_pane.cpp"
    "demos/chess/src/settings_pane.cpp"
    "demos/chess/src/lower_pane.cpp"
    "demos/chess/src/move_input.cpp"
    "demos/chess/src/piece.cpp"
    "demos/chess/src/move.cpp"
    )

set(GLYPH_PAINTER_SOURCES
    "demos/glyph_paint/glyph_paint.cpp"
    "demos/glyph_paint/paint_area.cpp"
    "demos/glyph_paint/side_pane.cpp"
    "demos/glyph_paint/attribute_box.cpp"
    "demos/glyph_paint/options_box.cpp"
    "demos/glyph_paint/populated_glyph_stack.cpp"
    )

set(DEMO_SOURCES
    "demos/demo.cpp"
    "demos/main_menu.cpp"
    "demos/notepad/notepad.cpp"
    ${CHESS_DEMO_SOURCES}
    ${GLYPH_PAINTER_SOURCES}
    )

add_library(cppurses STATIC
    ${SYSTEM_SOURCES}
	${PAINTER_SOURCES}
    ${WIDGET_SOURCES}
    )

# following block is copied from the GSL library
# when minimum version required is 3.8.0 remove if below
# both branches do exactly the same thing
if (CMAKE_MAJOR_VERSION VERSION_LESS 3.7.9)
    if (NOT MSVC)
        include(CheckCXXCompilerFlag)
        CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
        if(COMPILER_SUPPORTS_CXX14)
            target_compile_options(cppurses INTERFACE "-std=c++14")
        else()
            message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
        endif()

    endif()
else ()
    # set the cppurses library to be compiled only with c++14
    target_compile_features(cppurses INTERFACE cxx_std_14)
    # on *nix systems force the use of -std=c++XX instead of -std=gnu++XX (default)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# link ncurses and tinfo here so you only have to link cppursess at the application
target_link_libraries(cppurses INTERFACE
	ncurses
	tinfo
	)

target_compile_options(cppurses PUBLIC
    -Werror
    $<$<CXX_COMPILER_ID:Clang>:-ferror-limit=10>
    )

target_include_directories(cppurses PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/Signals/include
	${CMAKE_CURRENT_SOURCE_DIR}/libs/Signals/libs/Optional/include
	)

install(TARGETS cppurses
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Demos
add_executable(demo ${DEMO_SOURCES})
target_link_libraries(demo cppurses)

# Tests
# add_executable(testcppurses ${TEST_SOURCES})
# target_link_libraries(testcppurses cppurses ncurses)

# find_package(GTest REQUIRED)
# if (GTEST_FOUND)
#     target_link_libraries(testcppurses ${GTEST_BOTH_LIBRARIES})
# endif()

# add_test(testcppurses testcppurses)

# Build System Info For Clang-Tidy
if (NOT TARGET compile_commands)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    add_custom_target(compile_commands ALL
    	COMMAND [ -f ${CMAKE_SOURCE_DIR}/build/compile_commands.json ] && mv
            ${CMAKE_SOURCE_DIR}/build/compile_commands.json
    		${CMAKE_SOURCE_DIR}/compile_commands.json || :)
endif()

# Doxygen Target
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(docs
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
    COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif()
