
FILE(GLOB RESOURCE_FILES "*")

IF(APPLE)
  INSTALL(FILES ${RESOURCE_FILES} DESTINATION data)  
ELSE(APPLE)
  INSTALL(FILES ${RESOURCE_FILES} DESTINATION share/tankage/data)
ENDIF(APPLE)

