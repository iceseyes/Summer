set(HELLO_SRCS hello/hello.cc)

add_library(hello SHARED ${HELLO_SRCS})
set_target_properties(hello PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${SUMMER_WEBAPP_DIR}/hello"
)