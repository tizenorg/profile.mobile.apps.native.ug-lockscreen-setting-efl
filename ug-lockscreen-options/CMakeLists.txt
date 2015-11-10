CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ug-lockscreen-options C)

SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(LIBDIR "${PREFIX}/lib")

SET(VERSION 0.1.0)
SET(UGNAME "lockscreen-options")

SET(SRCS
	./src/lockscreen-options.c
	./src/lockscreen-options-util.c
	./src/lockscreen-options-main.c
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs_lock-screen-options REQUIRED elementary ui-gadget-1 evas capi-system-system-settings efl-extension appcore-efl capi-appfw-app-control)

FOREACH(flag ${pkgs_lock-screen-options_CFLAGS})
	SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} ${flag}")
ENDFOREACH(flag)



SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS} -fvisibility=hidden -Wall -Werror -fPIC")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g")
SET(CMAKE_C_FLAGS_RELEASE "-O2")

ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SOVERSION 0.1.0)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${pkgs_lock-screen-options_LDFLAGS})

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${LIBDIR})
