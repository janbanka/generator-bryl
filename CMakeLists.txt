cmake_minimum_required(VERSION 3.19)
project(generator_bryl LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools OpenGL OpenGLWidgets)

qt_standard_project_setup()

qt_add_executable(generator_bryl
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    myglwidget.cpp
    myglwidget.h
)

qt_add_translations(
    TARGETS generator_bryl
    TS_FILES generator_bryl_pl_PL.ts
)

target_link_libraries(generator_bryl
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::OpenGL
        Qt::OpenGLWidgets
)

target_include_directories(generator_bryl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

include(GNUInstallDirs)

install(TARGETS generator_bryl
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET generator_bryl
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
