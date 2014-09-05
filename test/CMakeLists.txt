set(TEST_SRCS
    main.cc 
    models/models.cc
)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_executable(runTest ${TEST_SRCS})

target_link_libraries(runTest 
    ${GTEST_LIBRARY} 
    ${GTEST_MAIN_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
)

add_test(
    NAME runTest
    COMMAND runTest
)