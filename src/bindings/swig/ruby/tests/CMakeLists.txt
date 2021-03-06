if (NOT (APPLE AND ENABLE_ASAN))
	file (GLOB TESTS testruby_*.rb)
endif (NOT (APPLE AND ENABLE_ASAN))

foreach (file ${TESTS})
	get_filename_component (name ${file} NAME_WE)
	add_test (NAME ${name} COMMAND ${RUBY_EXECUTABLE} ${file} --verbose)

	# set RUBYLIB to find newly built binding lib
	set_property (TEST ${name}
		      PROPERTY ENVIRONMENT
			       "RUBYLIB=${CMAKE_CURRENT_BINARY_DIR}/..:${CMAKE_CURRENT_SOURCE_DIR}/.."
			       "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/lib")

	set_property (TEST ${name} PROPERTY LABELS bindings memleak)

endforeach (file ${TESTS})

if (NOT (APPLE AND ENABLE_ASAN)) # special label for kdb
	set_property (TEST "testruby_kdb" APPEND PROPERTY LABELS kdbtests)
	set_property (TEST "testruby_kdb" PROPERTY RUN_SERIAL TRUE)

endif (NOT (APPLE AND ENABLE_ASAN))
