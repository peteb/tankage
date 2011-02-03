
# Finds ENet include directory and required libraries

UNSET(ENET_INCLUDE_DIR CACHE)
FIND_PATH(ENET_INCLUDE_DIR enet/enet.h
    HINTS /usr/include /usr/local/include $ENV{ENET_ROOT}/include
)

UNSET(ENET_LIBRARY CACHE)
FIND_LIBRARY(ENET_LIBRARY enet
	HINTS /usr/lib /usr/local/lib $ENV{ENET_ROOT}/lib
)

IF(NOT ENET_INCLUDE_DIR OR NOT ENET_LIBRARY)
    message(SEND_ERROR "ENet couln't be found")
ENDIF(NOT ENET_INCLUDE_DIR OR NOT ENET_LIBRARY)

