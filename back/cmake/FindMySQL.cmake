# - Find MySQL
# This module finds MySQL client library and headers
# Once done, this will define
#
#  MYSQL_FOUND        - System has MySQL
#  MYSQL_INCLUDE_DIRS - The MySQL include directories
#  MYSQL_LIBRARIES    - The libraries needed to use MySQL
#  MYSQL_VERSION_STRING - The version of MySQL found (since CMake 3.12)

find_path(MYSQL_INCLUDE_DIR mysql/mysql.h)
find_library(MYSQL_LIBRARY mysqlclient)

if (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
    set(MYSQL_FOUND TRUE)
else()
    set(MYSQL_FOUND FALSE)
endif()

if (MYSQL_FOUND)
    # Provide the include directories to the user
    set(MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR})
    
    # Provide the libraries to the user
    set(MYSQL_LIBRARIES ${MYSQL_LIBRARY})

    # Get MySQL version string
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(MySQL DEFAULT_MSG
        MYSQL_LIBRARIES MYSQL_INCLUDE_DIRS
    )
else()
    # Provide a helpful message if MySQL is not found
    message(STATUS "MySQL not found. Please set MYSQL_INCLUDE_DIR and MYSQL_LIBRARY.")
endif()

mark_as_advanced(MYSQL_INCLUDE_DIR MYSQL_LIBRARY)