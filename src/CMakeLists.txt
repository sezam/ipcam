# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.0)

if (MSVC)
    add_definitions(-D_WIN32_WINNT=0x600 -D_SCL_SECURE_NO_WARNINGS -D_WINSOCK_DEPRECATED_NO_WARNINGS)

    include_directories(
        d:/lib/vcpkg/installed/x64-windows/include/
    )

    link_directories(
        d:/lib/vcpkg/installed/x64-windows/lib/
    )
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wno-long-long -Wno-deprecated-declarations -Wno-unknown-pragmas")

    include_directories(
        /opt/vcpkg/installed/x64-linux/include/
    )

    link_directories(
        /opt/vcpkg/installed/x64-linux/lib
    )
endif()

file (GLOB COMMON_SRC *.cpp *.h)
set (SOURCE  ${COMMON_SRC})

find_package(Boost 1.72.0 REQUIRED COMPONENTS program_options)
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED COMPONENTS core imgcodecs videoio)

#Финальный штрих и сборка.
add_executable(${PACKAGE_TARNAME} ${SOURCE})

target_link_libraries(${PACKAGE_TARNAME}
    Boost::program_options 
    opencv_core
    opencv_imgcodecs
    opencv_videoio
)
