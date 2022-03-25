cmake_minimum_required(VERSION 3.0.0)
project(BinanceServer VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB BinanceServer_SRC
    #  "*.h"
    #  "*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

message("${TestLink_SRC}")

add_executable(BinanceServer main.cpp ${BinanceServer_SRC})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_include_directories(BinanceServer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")

target_include_directories(BinanceServer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/libbinacpp/include")

target_include_directories(BinanceServer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/libbinacpp/lib")

target_link_libraries(BinanceServer PUBLIC jsoncpp) # can use like -lws2_32 gcc
# -lcurl -lcrypto -lwebsockets -lbinacpp
target_link_libraries(BinanceServer PUBLIC curl)
# target_link_libraries(BinanceServer PUBLIC binacpp)
target_link_libraries(BinanceServer PUBLIC websockets)
target_link_libraries(BinanceServer PUBLIC crypto)
