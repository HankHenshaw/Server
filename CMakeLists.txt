cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(ENV{TRAVIS_BUILD_NUMBER} 1)
endif()

project(server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Threads)
find_package(Boost)

set(LIBFILES async.cpp async.h observer.cpp observer.h)

add_library(bulk_lib SHARED ${LIBFILES})
add_executable(bulkserver main.cpp server.h server.cpp)

set_target_properties(bulkserver PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wall;-Wextra;-Wpedantic"   
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(bulkserver
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    bulk_lib
)

install(TARGETS bulkserver RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT HankHenshaw at github.com)

include(CPack)