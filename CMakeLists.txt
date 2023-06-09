#*****************************************************************************
#   Copyright 2020 Embedded Office GmbH & Co. KG
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#******************************************************************************

#---
# preamble
#
cmake_minimum_required(VERSION 3.20)     # buildPresets is introduced in 3.20
project(canopen-linux)
enable_language(C ASM)

#---
# project wide setup
#
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS OFF)
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

#---
# externally provided content
#
include(cmake/CPM.cmake)
add_subdirectory(dependencies)

#---
# main targets built by this project
#
add_subdirectory(src)
