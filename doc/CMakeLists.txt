IF(BUILD_DOCUMENTATION)
 
  FIND_PACKAGE(Doxygen)
  IF(NOT DOXYGEN_FOUND)
    MESSAGE(FATAL_ERROR "Doxygen is needed to build the documentation.")
  ENDIF()
 
  SET( doxyfile_in          Doxyfile.in                                 )
  SET( doxyfile             Doxyfile                                    )
  SET( doxy_html_index_file ${CMAKE_CURRENT_BINARY_DIR}/html/index.html )
  
  SET( doxy_output_root     ${CMAKE_CURRENT_BINARY_DIR}) # Pasted into Doxyfile.in
  SET( doxy_extra_files     ) # Pasted into Doxyfile.in
  
  CONFIGURE_FILE( ${doxyfile_in} ${doxyfile} @ONLY )
 
  ADD_CUSTOM_COMMAND( OUTPUT ${doxy_html_index_file}
                      COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
                      MAIN_DEPENDENCY ${doxyfile} ${doxyfile_in}
                      DEPENDS SummerServer ${doxy_extra_files}
                      COMMENT "Generating HTML documentation")
 
  ADD_CUSTOM_TARGET( doc ALL DEPENDS ${doxy_html_index_file} )
 
  INSTALL( DIRECTORY ${doxy_output_root}/html DESTINATION share/doc )
ENDIF()
