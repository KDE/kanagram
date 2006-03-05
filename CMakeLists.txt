kde4_header()

add_subdirectory( data ) 
add_subdirectory( images ) 
add_subdirectory( src ) 
add_subdirectory( icons ) 
add_subdirectory( sounds ) 

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = data images src icons sounds
