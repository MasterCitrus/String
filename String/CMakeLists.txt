add_library(String STATIC CString.cpp)

target_include_directories(String PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
