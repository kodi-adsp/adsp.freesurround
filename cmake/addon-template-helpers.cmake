# Function:                 EXCLUDE_FILES_FROM_DIR_IN_LIST
#                           Inspired by http://stackoverflow.com/a/24509333
# Description:              Exclude all files from a list under a specific directory.
# Param _InFileList:        Input and returned List 
# Param _excludeDirName:    Name of the directory, which shall be ignored.
# Param _verbose:           Print the names of the files handled
#
# Example: EXCLUDE_FILES_FROM_DIR_IN_LIST("${SOURCE_FILES}" "/hide/" FALSE)
function(EXCLUDE_FILES_FROM_DIR_IN_LIST _InFileList _excludeDirName _verbose)
  foreach (ITR ${_InFileList})
    if ("${_verbose}")
      message(STATUS "ITR=${ITR}")
    endif ("${_verbose}")

    if ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)")                   # Check if the item matches the directory name in _excludeDirName
      message(STATUS "Remove Item from List:${ITR}")
      list (REMOVE_ITEM _InFileList ${ITR})                              # Remove the item from the list
    endif ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)")

  endforeach(ITR)
  set(SOURCE_FILES ${_InFileList} PARENT_SCOPE)                          # Return the SOURCE_FILES variable to the calling parent
endfunction(EXCLUDE_FILES_FROM_DIR_IN_LIST)


function(addon_source_group FILE_LIST)
  foreach(_FILE ${FILE_LIST})
    if(IS_ABSOLUTE ${_FILE})
      get_filename_component(${_FILE} ${_FILE} ABSOLUTE)
      #file(RELATIVE_PATH relFile ${CMAKE_CURRENT_SOURCE_DIR} ${_FILE})
      get_filename_component(FILEDIRECTORY ${_FILE} DIRECTORY)
      #message(STATUS "FILEDIRECTORY=${FILEDIRECTORY}")
      file(RELATIVE_PATH FILEDIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} ${FILEDIRECTORY})
    else()
      #message(STATUS "FILEDIRECTORY=${FILEDIRECTORY}")
      get_filename_component(FILEDIRECTORY ${_FILE} DIRECTORY)
    endif()

    string(REPLACE "asplib/" "" FILEDIRECTORY "${FILEDIRECTORY}")

    #message(STATUS "_FILE=${_FILE}")
    if(NOT "${FILEDIRECTORY}" STREQUAL "")
      string(REPLACE "/" "\\" FILE_FILEGROUP ${FILEDIRECTORY})
    else()
      set(FILE_FILEGROUP "")
    endif()
    get_filename_component(FILENAME ${_FILE} NAME)
    
    #message(STATUS "Filename: ${FILENAME}, Directory: ${FILEDIRECTORY}, Group: ${FILE_FILEGROUP}")
    
    source_group("${FILE_FILEGROUP}" FILES ${_FILE})
  endforeach()
endfunction(addon_source_group)
