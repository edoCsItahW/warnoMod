if (NOT DEFINED Node_DIR)
    set(Node_DIR "C:/Users/Lenovo/AppData/Local/node-gyp/Cache/20.12.2")
endif ()

if (NOT DEFINED Node_INCLUDE_DIRS)
    set(Node_INCLUDE_DIRS ${Node_DIR}/include/node)
endif ()

if (NOT DEFINED Node_LIBRARIES)
    set(Node_LIBRARIES ${Node_DIR}/x64/node.lib)
endif ()

if (NOT DEFINED NAPI_INCLUDE_DIRS)
    set(NAPI_INCLUDE_DIRS "D:/ProgramFiles/CACHE/pnpm/global/5/node_modules/node-addon-api")
endif ()

MESSAGE(STATUS "node.cmake <Node_DIR>: ${Node_DIR}")
MESSAGE(STATUS "node.cmake <Node_INCLUDE_DIRS>: ${Node_INCLUDE_DIRS}")
MESSAGE(STATUS "node.cmake <Node_LIBRARIES>: ${Node_LIBRARIES}")
MESSAGE(STATUS "node.cmake <NAPI_INCLUDE_DIRS>: ${NAPI_INCLUDE_DIRS}")

include_directories(${Node_INCLUDE_DIRS})
include_directories(${NAPI_INCLUDE_DIRS})

#target_link_libraries(jsExtent PRIVATE ${Node_DIR}/x64/node.lib)
