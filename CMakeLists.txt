cmake_minimum_required(VERSION 3.16)

project(POFMS VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Qt path
set(CMAKE_PREFIX_PATH "D:/Softwares/QT/6.9.0/msvc2022_64" ${CMAKE_PREFIX_PATH})
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
if(Qt6_FOUND)
    message("Qt6 found at: ${Qt6_DIR}")
else()
    message(FATAL_ERROR "Qt6 not found. Please verify the path D:/Softwares/QT/6.9.0/msvc2022_64")
endif()

# Set OpenCV path
find_package(OpenCV REQUIRED PATHS "D:/Softwares/opencv/install" NO_DEFAULT_PATH)
include_directories(${OpenCV_INCLUDE_DIRS})
if(OpenCV_FOUND)
    message("OpenCV found at: ${OpenCV_DIR}")
    message("OpenCV libraries: ${OpenCV_LIBS}")
    message("OpenCV library dir: ${OpenCV_LIB_DIR}")
else()
    message(FATAL_ERROR "OpenCV not found. Please verify the path D:/Softwares/opencv/install")
endif()

# Explicitly set OpenCV library directory
set(OpenCV_LIB_DIR "D:/Softwares/opencv/install/x64/vc17/lib")
link_directories(${OpenCV_LIB_DIR})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        main.h
        user.h
        usermanager.h
        loginwindow.h
        loginwindow.cpp
        loginwindow.ui
        usermanager.cpp
        employee.h
        employeemanager.h
        employeemanager.cpp
        employee.cpp
        item.h
        item.cpp
        inventorymanager.h
        inventorymanager.cpp
        productionlog.h
        productionlog.cpp
        productionlogmanager.h
        productionlogmanager.cpp
        maintenancerecord.h
        maintenancerecord.cpp
        maintenancemanager.h
        maintenancemanager.cpp
        introwindow.h
        introwindow.cpp
        introwindow.ui
        facerecognitionwindow.h
        facerecognitionwindow.cpp
        facerecognitionwindow.ui
)

qt_add_executable(POFMS
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

# Determine library suffix based on build type
set(OpenCV_LIB_SUFFIX "")
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(OpenCV_LIB_SUFFIX "d")
endif()
message("Using OpenCV library suffix: ${OpenCV_LIB_SUFFIX}")

# Explicitly link OpenCV libraries with full paths
target_link_libraries(POFMS PRIVATE
    Qt6::Widgets
    "${OpenCV_LIB_DIR}/opencv_core4110${OpenCV_LIB_SUFFIX}.lib"
    "${OpenCV_LIB_DIR}/opencv_imgproc4110${OpenCV_LIB_SUFFIX}.lib"
    "${OpenCV_LIB_DIR}/opencv_objdetect4110${OpenCV_LIB_SUFFIX}.lib"
    "${OpenCV_LIB_DIR}/opencv_videoio4110${OpenCV_LIB_SUFFIX}.lib"
    "${OpenCV_LIB_DIR}/opencv_imgcodecs4110${OpenCV_LIB_SUFFIX}.lib"
    "${OpenCV_LIB_DIR}/opencv_face4110${OpenCV_LIB_SUFFIX}.lib"
)

set_target_properties(POFMS PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS POFMS
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(POFMS)
