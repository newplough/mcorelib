PROJECT(mcorelib)
cmake_minimum_required(VERSION 3.0)

set(LIB_NAME mcorelib)


SET(LIB_SRCS
    ../sources/mapplication.cpp
    ../sources/mappevent.cpp
    ../sources/mlogger.cpp
    ../sources/mobject.cpp
    ../sources/mobjectpool.cpp
    ../sources/mplatform.cpp
    ../sources/mthread.cpp
    ../sources/mtimer.cpp
    ../sources/mtimertask.cpp
    ../sources/mvariant.cpp
    ../sources/mmutex.cpp
    ../sources/msemaphore.cpp
    ../sources/mplatform.cpp
)

SET(INC_DIR 
    ../include
)


INCLUDE_DIRECTORIES(${INC_DIR}) 


add_library(${LIB_NAME} STATIC ${LIB_SRCS})

IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set (DEP_LIB mcorelib)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set (DEP_LIB mcorelib pthread)
ENDIF ()


add_executable(test_variant ../testor/test_variant_slot.cpp)
target_link_libraries(test_variant ${LIB_NAME} ${DEP_LIB})

add_executable(test_timer ../testor/test_timer_slot.cpp)
target_link_libraries(test_timer ${LIB_NAME} ${DEP_LIB})
