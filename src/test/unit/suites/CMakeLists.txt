MACRO(ADD_MODULE file)
    ADD_LIBRARY( ${file} MODULE ${file}.c ../utils.c ../panic.c ../kern.c )
#    TARGET_LINK_LIBRARIES( ${file} cunit kern phantom win )
    TARGET_LINK_LIBRARIES( ${file} cunit openpod_helpers )
    SET_TARGET_PROPERTIES( ${file} PROPERTIES
            PREFIX ""
            LIBRARY_OUTPUT_DIRECTORY "."
    )   
ENDMACRO(ADD_MODULE file)

FILE(GLOB C_FILES RELATIVE "${CMAKE_SOURCE_DIR}/suites" "${CMAKE_SOURCE_DIR}/suites/*.c")

INCLUDE_DIRECTORIES ( "${CMAKE_SOURCE_DIR}" )
INCLUDE_DIRECTORIES ( "../../../openpod" ".." )


SET(CMAKE_C_FLAGS " -std=c99 -O3 -Wall -Wextra -Wimplicit  -I ../../../openpod  -L../../../libs/")


FOREACH ( module ${C_FILES} )
    STRING( REGEX REPLACE ".c$" "" module "${module}" )
    MESSAGE(STATUS "Found test suite: ${module}")
    ADD_MODULE(${module})
ENDFOREACH ( module ${MODULES} )
