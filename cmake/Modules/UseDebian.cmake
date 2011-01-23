
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Peter & Co")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A 2D game where tow snails fight to the death.")

SET(CPACK_PACKAGE_NAME "snail-wail")

SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "1")


# FIXME kaspars: Make debian package name to follow common naming convention
# <foo>_<VersionNumber>-<DebianRevisionNumber>_<DebianArchitecture>.deb
SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

INCLUDE(CPack)

