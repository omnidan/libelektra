# ElektraPackaging needs to be included first (sets PACKAGE_ variables)

include (CMakePackageConfigHelpers)

# cmake settings for use by find_package
configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/ElektraConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/ElektraConfig.cmake" @ONLY)

write_basic_package_version_file (
	"${CMAKE_CURRENT_BINARY_DIR}/ElektraConfigVersion.cmake" VERSION ${KDB_VERSION} COMPATIBILITY AnyNewerVersion)

install (FILES "${CMAKE_CURRENT_BINARY_DIR}/ElektraConfig.cmake"
	       "${CMAKE_CURRENT_BINARY_DIR}/ElektraConfigVersion.cmake"
	       DESTINATION
	       ${TARGET_CMAKE_FOLDER})

# pkg-config files
configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/elektra.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/elektra.pc" @ONLY)

install (FILES "${CMAKE_CURRENT_BINARY_DIR}/elektra.pc" DESTINATION lib${LIB_SUFFIX}/${TARGET_PKGCONFIG_FOLDER})

add_custom_target (uninstall
		   COMMAND "${CMAKE_COMMAND}" -D KDB_VERSION=${KDB_VERSION} -D CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -D
			   CMAKE_FIND_LIBRARY_PREFIXES=${CMAKE_FIND_LIBRARY_PREFIXES} -D
			   CMAKE_FIND_LIBRARY_SUFFIXES=${CMAKE_FIND_LIBRARY_SUFFIXES} -P
			   "${CMAKE_CURRENT_SOURCE_DIR}/ElektraUninstall.cmake"
		   WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
		   COMMENT "Uninstalling Elektra from system or DESTDIR")
