# Mirai++ API Library

set(LIB_NAME miraipp)

set(SOURCE_FILES
    "mirai/core/session.cpp" "mirai/core/common.cpp"
    "mirai/core/events.cpp" "mirai/core/types.cpp"
    "mirai/core/message/message.cpp" "mirai/core/message/segment.cpp"
    "mirai/core/message/common.cpp" "mirai/core/message/received_message.cpp"
    "mirai/core/websockets/client.cpp" "mirai/core/websockets/connection.cpp"
    "mirai/utils/thread.cpp" "mirai/utils/encoding.cpp"
    "mirai/utils/request.cpp"
) 

add_library(${LIB_NAME} ${SOURCE_FILES})
target_include_directories(${LIB_NAME} INTERFACE ./)

target_compile_features(${LIB_NAME} PRIVATE cxx_std_17) # Enforce C++17 or newer
set_target_properties(${LIB_NAME} PROPERTIES CXX_EXTENSIONS off)
target_compile_definitions(${LIB_NAME} PUBLIC ASIO_STANDALONE)

if (MSVC)
    # Force MSVC to use utf-8 encoding regardless of whether the BOM exists
    target_compile_options(${LIB_NAME} PRIVATE "/utf-8")
    # /Zc:__cplusplus is needed for MSVC to produce correct value for the __cplusplus macro
    # If this option is not on, __cplusplus will always expands to 199711L
    if (MSVC_VERSION GREATER_EQUAL 1914)
        target_compile_options(${LIB_NAME} PUBLIC "/Zc:__cplusplus")
    endif ()
endif ()

# Warnings and errors settings
# Use highest reasonable warning level, and treat warnings as errors
if (MSVC) # Visual Studio
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]") # If default /W3 presents
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # Change /W3 to /W4
    else ()
        target_compile_options(${LIB_NAME} PRIVATE /W4) # Add /W4 directly
    endif ()
    target_compile_options(${LIB_NAME} PRIVATE /WX) # Treat warnings as errors
    # Treat all header files specified by angle brackets to be system headers, and ignore all those warnings
    target_compile_options(${LIB_NAME} PRIVATE 
        /experimental:external /external:W0 /external:anglebrackets)
else () # Not Visual Studio, assuming gcc or clang
    target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif ()

find_package(cpr CONFIG REQUIRED)
find_package(websocketpp CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(unofficial-iconv CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${LIB_NAME} PRIVATE
    cpr
    unofficial::iconv::libiconv unofficial::iconv::libcharset)
target_link_libraries(${LIB_NAME} PUBLIC
    websocketpp::websocketpp
    OpenSSL::SSL OpenSSL::Crypto
    asio asio::asio
    nlohmann_json nlohmann_json::nlohmann_json)
