project(SummerServer)
cmake_minimum_required(VERSION 2.8)

set(SERVER_SRCS
    src/logger.cc
    
    src/server/Application.cc
    src/server/WebAppFolder.cc
    src/server/mime_types.cc
    src/server/UrlHandler.cc
    src/server/Request.cc src/server/Reply.cc
    src/server/ApplicationRegistry.cc
    src/server/URL.cc src/server/URI.cc
)

set(SRCS
    src/main.cc
)

set(SUMMER_WEBAPP_DIR ${CMAKE_BINARY_DIR}/webapps/)

message(STATUS ${CMAKE_PREFIX_PATH})

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

find_package(PkgConfig REQUIRED)
find_package(Log4CPP REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost 1.55.0 REQUIRED 
    unit_test_framework system regex date_time thread filesystem
    program_options chrono atomic )

pkg_check_modules(GLIBMM glibmm-2.4)

set(BOOST_LIBS
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_DATE_TIME_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_CHRONO_LIBRARY}
)        

if(LOG4CPP_FOUND)
    message(STATUS "Log4CPP trovato!")
    add_definitions(-DLOGGER)
    include_directories(${LOG4CPP_INCLUDE_DIR})
    if(WIN32)
        set(LOGGER_LIBS ${LOG4CPP_LIBRARIES}/liblog4cpp.a)
    else(WIN32)
        set(LOGGER_LIBS log4cpp nsl)
    endif(WIN32)    
else()
    message(WARNING "Log4CPP non � stato trovato")
endif()

if (WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501)
  set(WINLIBS wsock32 ws2_32)
endif(WIN32)
add_definitions(-DBOOST_NETWORK_ENABLE_HTTPS)

link_directories(${GLIBMM_LIBRARY_DIRS})

include_directories(include)
include_directories(${GLIBMM_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIRS})
include_directories(${MYSQLCONNECTORCPP_INCLUDE_DIR})

add_library(SummerServer ${SERVER_SRCS})

add_executable(Summer ${SRCS})
target_link_libraries ( Summer
    SummerServer
    ${GLIBMM_LIBRARIES}
    ${LOGGER_LIBS}
    ${BOOST_LIBS} 
    ${CMAKE_THREAD_LIBS_INIT}
    ${WINLIBS}
)

add_subdirectory(examples) 