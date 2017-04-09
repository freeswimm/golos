file(GLOB HEADERS "include/steemit/time/*.hpp")

if(BUILD_SHARED_LIBRARIES)
    add_library(golos_time SHARED
            time.cpp
            )
else()
    add_library(golos_time STATIC
            time.cpp
            )
endif()

target_link_libraries(golos_time fc)
target_include_directories(golos_time
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

install(TARGETS
        golos_time

        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )
install(FILES ${HEADERS} DESTINATION "include/steemit/time")
