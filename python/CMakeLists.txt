project(mult)
cmake_minimum_required(VERSION 3.10)

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++17" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++17")
    endif()
endif()

add_subdirectory(thirdparty/pybind11)
pybind11_add_module(mult main.cpp)
