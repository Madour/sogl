macro(configure_policies)
    if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
        #  Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP
        message(STATUS "[sogl] Using policy CMP0135 NEW")
        cmake_policy(SET CMP0135 NEW) # sets the timestamps of extracted contents to the time of extraction
    endif()
endmacro()
