
# Finds cURL include directory and required libraries

UNSET(CURL_INCLUDE_DIR CACHE)
FIND_PATH(CURL_INCLUDE_DIR curl/curl.h
    HINTS /usr/include /usr/local/include $ENV{CURL_ROOT}/include
)

UNSET(CURL_LIBRARY CACHE)
FIND_LIBRARY(CURL_LIBRARY curl
	HINTS /usr/lib /usr/local/lib $ENV{CURL_ROOT}/lib
)

IF(NOT CURL_INCLUDE_DIR OR NOT CURL_LIBRARY)
    message(SEND_ERROR "cURL couln't be found")
ELSE(NOT CURL_INCLUDE_DIR OR NOT CURL_LIBRARY)
    message(STATUS "Found cURL: ${CURL_INCLUDE_DIR} ${CURL_LIBRARY}")
ENDIF(NOT CURL_INCLUDE_DIR OR NOT CURL_LIBRARY)


