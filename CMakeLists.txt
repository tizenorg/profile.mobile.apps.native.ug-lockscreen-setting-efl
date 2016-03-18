CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ug-lockscreen-options C)

ADD_SUBDIRECTORY(res/po)

SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(BINDIR "${PREFIX}/bin")

SET(VERSION 0.1.0)
#SET(UGNAME "lockscreen-options")

SET(SRCS
	./src/lockscreen-options.c
	./src/lockscreen-options-util.c
	./src/lockscreen-options-main.c
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/inc)
INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs_lock-screen-options REQUIRED elementary evas capi-system-system-settings efl-extension appcore-efl capi-appfw-application capi-appfw-app-control libtzplatform-config dlog vconf)

FOREACH(flag ${pkgs_lock-screen-options_CFLAGS})
	SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} ${flag}")
ENDFOREACH(flag)


SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS} -fvisibility=hidden -Wall -Werror -fPIC")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g")
SET(CMAKE_C_FLAGS_RELEASE "-O2")

#ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS})
ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SOVERSION 0.1.0)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${pkgs_lock-screen-options_LDFLAGS})

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${BINDIR})

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/tizen-manifest.xml DESTINATION ${TZ_SYS_RO_PACKAGES} RENAME ${PROJECT_NAME}.xml)
