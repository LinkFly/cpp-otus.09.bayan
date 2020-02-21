cmake_minimum_required(VERSION 3.2)
message("[INFO] Using env variable `TRAVIS_BUILD_NUMBER` = $ENV{TRAVIS_BUILD_NUMBER}")
message("[INFO] Using env variable `BOOST_ROOT` = $ENV{BOOST_ROOT}")
message("[INFO] Using env variable `BOOST_INCLUDEDIR` = $ENV{BOOST_INCLUDEDIR}")
message("[INFO] Using env variable `BOOST_LIBRARYDIR` = $ENV{BOOST_LIBRARYDIR}")
project(bayan VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

set(SOURCES
	main.cpp
	)
set(HEADERS
	share.h
	logger.h
	Config.h
	Block.h
	Hash.h
	EqualGroup.h
	FileReaded.h
	FilesUtils.h
	Bayan.h
	Arguments.h
	)
set(TESTS_HEADERS

	)
set(DATA_FILES
	Readme.md

	)
option(LOGS "Enable output logs" false)
configure_file(configure.h.in configure.h)

add_executable(bayan ${SOURCES} ${HEADERS} ${DATA_FILES})
add_executable(tests tests.cpp ${TESTS_HEADERS} ${HEADERS})
set_target_properties(bayan tests PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON
	)

find_package(Boost 1.71 COMPONENTS unit_test_framework filesystem program_options REQUIRED)

IF(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
message("[INFO] Using MSVC")
target_link_libraries(bayan
	${Boost_FILESYSTEM_LIBRARY}
)
ELSE()
message("[INFO] Using not MSVC")
target_link_libraries(bayan
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
)
ENDIF()

#target_link_libraries(bayan
# Disable all boost libraries link (will be concrete linking)
	#${Boost_LIBRARIES}
#	${Boost_FILESYSTEM_LIBRARY}
# options_library don't work as dynamic library
	#${Boost_PROGRAM_OPTIONS_LIBRARY}
#)

target_link_libraries(tests
	${Boost_LIBRARIES}
)

message("[INFO] Boost libs: ${Boost_LIBRARIES}")
message("[INFO] Boost inc: ${Boost_INCLUDE_DIR}")
message("[INFO] Boost inc dirs: ${Boost_INCLUDE_DIRS}")
message("[INFO] Boost lib dirs: $ENV{BOOST_LIBRARYDIR}")

set_target_properties(bayan tests PROPERTIES
	COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
	INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS}
	LINK_DIRECTORIES $ENV{BOOST_LIBRARYDIR}
)

IF(WIN32)
set_target_properties(bayan tests PROPERTIES
  #COMPILE_OPTIONS "-Wall" # A lot of messages (in std libraries)
  COMPILE_OPTIONS ""
)
ELSE()
set_target_properties(bayan tests PROPERTIES
	#COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Wno-c++11-extensions"
	COMPILE_OPTIONS ""
)
ENDIF()

### Tests ###
enable_testing()
add_test(bayan_tests tests)
###

### Packaging ###
install(TARGETS bayan RUNTIME DESTINATION bin)
install(TARGETS tests RUNTIME DESTINATION lib/bayan/tests)
install(TARGETS bayan-orig RUNTIME DESTINATION bin)
install(TARGETS tests-orig RUNTIME DESTINATION lib/bayan/tests)
install(FILES build/bayan-config.conf DESTINATION bin)
install(FILES build/tests-config.conf DESTINATION lib/bayan/tests)
install(DIRECTORY build/boost_factory/usr/lib/boost_1_72_0 DESTINATION bin/boost_factory/usr/lib/boost_1_72_0)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT linkfly@yandex.ru)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
