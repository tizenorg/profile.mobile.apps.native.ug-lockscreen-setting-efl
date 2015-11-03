CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ug-lockscreen-options C)

ADD_SUBDIRECTORY(ug-lockscreen-options)
ADD_SUBDIRECTORY(po)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/ug-lockscreen-options.xml DESTINATION /usr/share/packages/)