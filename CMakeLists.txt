kde4_header()
project(kanagram)

add_subdirectory( data ) 
add_subdirectory( images ) 
add_subdirectory( src ) 
add_subdirectory( icons ) 
add_subdirectory( sounds ) 




########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = data images src icons sounds
