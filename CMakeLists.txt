cmake_minimum_required(VERSION 3.5)

project(kn VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SerialPort)

set(PROJECT_SOURCES
    arrow.h
    arrow.cpp
    diagramitem.h
    diagramitem.cpp
    diagramscene.h
    diagramitem.cpp
    diagramscene.cpp
    diagramtextitem.h
    diagramtextitem.cpp
    main.cpp
    mainwindow.h
    mainwindow.cpp
    settingsform.h
    settingsform.cpp
)

set(PROJECT_RESOURCES
    diagramscene.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(kn
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        RESOURCES
        ${PROJECT_RESOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET kn APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

else()
    if(ANDROID)
        qt5_add_resources(PROJECT_SOURCES ${PROJECT_RESOURCES})
        add_library(kn SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        qt5_add_resources(PROJECT_SOURCES ${PROJECT_RESOURCES})
        add_executable(kn
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(kn PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::SerialPort)

set_target_properties(kn PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER kn.elmas.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS kn
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(kn)
endif()
