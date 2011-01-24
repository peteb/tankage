
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Peter & Co")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A 2D game where tow snails fight to the death.")

SET(CPACK_PACKAGE_NAME "snail-wail")

SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")

# From the maintainers guide package should have following naming convention
# <foo>_<VersionNumber>-<DebianRevisionNumber>_<DebianArchitecture>.deb
SET(CPACK_PACKAGE_FILE_NAME 
	"${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}-1_i383"
)

set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA 
	"${PROJECT_ROOT_DIR}/cmake/Scripts/preinst;${PROJECT_ROOT_DIR}/cmake/Scripts/postrm;"
)

# FIXME kaspars: Resolve dependencies!
# SET(CPACK_DEBIAN_PACKAGE_DEPENDS "pacman (>= 10-17)")

INCLUDE(CPack)

