#
#  Copyright (C) 2014 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of TWS.
#
#  TWS is free software: you can
#  redistribute it and/or modify it under the terms of the
#  GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License,
#  or (at your option) any later version.
#
#  TWS is distributed in the hope that
#  it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with TWS. See LICENSE. If not, write to
#  e-sensing team at <esensning-team@dpi.inpe.br>.
#
#
#  CMake scripts for TerraLib Web Services
#
#  Description: Script for generating the build of Web Coverage Service support.
#
#  Author: Raphael Willian da Costa

include_directories(${RAPIDJSON_INCLUDE_DIR})
include_directories(${RAPIDXML_INCLUDE_DIR})
include_directories(${terralib_INCLUDE_DIRS})

file(GLOB TWS_SRC_FILES ${TWS_ABSOLUTE_ROOT_DIR}/src/tws/wcs/*.cpp)
file(GLOB TWS_HDR_FILES ${TWS_ABSOLUTE_ROOT_DIR}/src/tws/wcs/*.hpp)

add_library(tws_mod_wcs SHARED ${TWS_SRC_FILES} ${TWS_HDR_FILES})

target_link_libraries(tws_mod_wcs tws_mod_geoarray
                                  terralib_mod_plugin
                                  terralib_mod_raster
                                  terralib_mod_srs
                                  ${Boost_FILESYSTEM_LIBRARY}
                                  ${Boost_SYSTEM_LIBRARY})

set_target_properties(tws_mod_wcs
                      PROPERTIES VERSION ${TWS_VERSION_MAJOR}.${TWS_VERSION_MINOR}
                                 SOVERSION ${TWS_VERSION_MAJOR}.${TWS_VERSION_MINOR})

install(TARGETS tws_mod_wcs
        EXPORT tws-targets
        RUNTIME DESTINATION ${TWS_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TWS_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TWS_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TWS_HDR_FILES}
        DESTINATION ${TWS_DESTINATION_HEADERS}/tws/wcs COMPONENT devel)

export(TARGETS tws_mod_wcs APPEND FILE ${CMAKE_BINARY_DIR}/tws-exports.cmake)
