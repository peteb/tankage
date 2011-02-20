
FILE(GLOB RESOURCE_FILES "*.png")
INSTALL(FILES ${RESOURCE_FILES} DESTINATION share/snail-wail/data)
INSTALL(FILES "snail-wail.cfg" DESTINATION share/snail-wail/data)

