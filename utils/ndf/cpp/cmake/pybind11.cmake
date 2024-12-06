if (NOT pybind11_DIR)
    set(pybind11_DIR "E:/ProgramFiles/Anaconda/Lib/site-packages/pybind11/share/cmake/pybind11")
endif()

MESSAGE(STATUS "pybind11.cmake <pybind11_DIR>: ${pybind11_DIR}")

find_package(pybind11 REQUIRED CONFIG)

if (pybind11_INCLUDE_DIRS)
    MESSAGE(STATUS "pybind11.cmake <pybind11_INCLUDE_DIRS>: ${pybind11_INCLUDE_DIRS}")
    include_directories(${pybind11_INCLUDE_DIRS})
    MESSAGE(STATUS "pybind11.cmake <NOTE>: The library '${pybind11_INCLUDE_DIRS}' has been automatically included for you")
else ()
    MESSAGE(FATAL_ERROR "pybind11.cmake <pybind11_INCLUDE_DIRS> not found")
endif ()
