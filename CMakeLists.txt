CMAKE_MINIMUM_REQUIRED (VERSION 3.0)

PROJECT(CYTOK)


###
## Configure Files
###

set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x")

SET(CK_ROOT_DIR ${CMAKE_SOURCE_DIR})

SET(CK_LIB_DIR ${CMAKE_SOURCE_DIR}/builds/)
SET(CK_INC_DIR ${CMAKE_SOURCE_DIR}/cytok/include/)

SET(CK_UTT_DIR ${CMAKE_SOURCE_DIR}/unit_test/)
SET(CK_UTT_RESULTS_DIR ${CMAKE_SOURCE_DIR}/unit_test/results/)

SET(CK_DEVT_DIR ${CMAKE_SOURCE_DIR}/devt/)

CONFIGURE_FILE(cmake_config/dev_paths.hpp ${CMAKE_SOURCE_DIR}/devt/source/)
CONFIGURE_FILE(cmake_config/unit_paths.hpp ${CMAKE_SOURCE_DIR}/unit_test/source/)

##############################
## Libraries


FIND_PACKAGE( Boost REQUIRED )
FIND_PACKAGE( CppUnit REQUIRED )
#FIND_PACKAGE( TinyXML REQUIRED )



##############################

###
## Files
###

## Master includes
FILE(	
	GLOB_RECURSE 
	ck_include 
	cytok/include/*
	)

## Master sources
FILE(
	GLOB_RECURSE 
	ck_src 
	cytok/src/*
	)

## Assets
FILE(
	GLOB_RECURSE 
	ck_assets 
	cytok/assets/*
	)
	

###
## Library Target
###

ADD_LIBRARY(
	Cytok SHARED 
	${ck_include} 
	${ck_src}
	)


###
## Properties
###

## Set includes dir (master header and module headers)
SET_PROPERTY(
	TARGET Cytok
	PROPERTY INCLUDE_DIRECTORIES
  	${CMAKE_SOURCE_DIR}/cytok/include/
  	${Boost_INCLUDE_DIR}
)


## Set Target Library
TARGET_LINK_LIBRARIES(
	Cytok
	${Boost_LIBRARIES}
	)


## Set output dir (builds)
SET_PROPERTY( TARGET Cytok PROPERTY LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/builds/)
SET_PROPERTY( TARGET Cytok PROPERTY LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/builds/)
SET_PROPERTY( TARGET Cytok PROPERTY LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/builds/)
## Set output dir 


###
## Unit Test
###

FILE(
	GLOB_RECURSE
	unit_sources
	unit_test/source/*
	)

## Master Tests

ADD_EXECUTABLE(
	UnitMaster 
	${unit_sources} 
	${ck_include} 
	${ck_src}
	)


## Set output dir 
SET_PROPERTY(TARGET UnitMaster PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/unit_test/builds/ )
SET_PROPERTY(TARGET UnitMaster PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/unit_test/builds/ )
SET_PROPERTY(TARGET UnitMaster PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/unit_test/builds/ )

	
## Set includes dir (master header and module headers)
SET_PROPERTY(
	TARGET UnitMaster 
	PROPERTY INCLUDE_DIRECTORIES
  	${CMAKE_SOURCE_DIR}/cytok/include/
  	${CMAKE_SOURCE_DIR}/unit_test/include/
  	${Boost_INCLUDE_DIR}
  	${CPPUNIT_INCLUDE_DIRS}
)


## Set Target Library
TARGET_LINK_LIBRARIES(
	UnitMaster
	${Boost_LIBRARIES}
	${CPPUNIT_LIBRARIES}
	)
	


###
## Dev Test
###

FILE(
	GLOB_RECURSE
	devt_sources
	devt/source/*
	)

## Master Tests

ADD_EXECUTABLE(
	DevTest 
	${devt_sources} 
	)


## Set output dir 
SET_PROPERTY(TARGET DevTest PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devt/builds/ )
SET_PROPERTY(TARGET DevTest PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/devt/builds/ )
SET_PROPERTY(TARGET DevTest PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/devt/builds/ )

	
## Set includes dir (master header and module headers)
SET_PROPERTY(
	TARGET DevTest 
	PROPERTY INCLUDE_DIRECTORIES
	${CMAKE_SOURCE_DIR}/cytok/include/
  	${CMAKE_SOURCE_DIR}/devt/include/
  	${Boost_INCLUDE_DIR}
  	${CPPUNIT_INCLUDE_DIRS}
)


## Set Target Library
TARGET_LINK_LIBRARIES(
	DevTest
	Cytok
	${Boost_LIBRARIES}
	${CppUnit_LIBRARIES}
	)

####################

###
## Organize
###

SOURCE_GROUP(
	Dev_Tests
	FILES ${devt_sources}
	)

SOURCE_GROUP(
	Unit_Tests
	FILES ${unit_sources}
	)



SOURCE_GROUP(
	Headers
	FILES ${ck_include}
	)
	
SOURCE_GROUP(
	Sources
	FILES ${ck_src}
	)
	
	
