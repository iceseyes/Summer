set(MINIMAL_SRCS minimal/minimal.cc)
set(HELLO_SRCS hello/hello.cc)
set(HELLO_WEB hello/web)

add_library(hello SHARED ${HELLO_SRCS})
add_library(minimal SHARED ${MINIMAL_SRCS})

set_target_properties(hello PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${SUMMER_WEBAPP_DIR}/hello"
)
set_target_properties(minimal PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${SUMMER_WEBAPP_DIR}/minimal"
)

file(COPY hello/web DESTINATION "${SUMMER_WEBAPP_DIR}/hello")
