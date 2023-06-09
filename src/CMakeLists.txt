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
# define the application executable
#
add_executable(canopen-linux)

#---
# application with object dictionary and driver functions
#
target_sources(canopen-linux
  PRIVATE
    # application
    app/clock_app.c
    app/clock_hw.c
    app/clock_spec.c
    # config
    config/main.cpp
    # driver
    driver/linux/drv_can_socketcan.cpp
    driver/linux/drv_nvm_file.cpp
    driver/linux/drv_timer_cpp.cpp
)

target_include_directories(canopen-linux
  PRIVATE
    app
    config
    driver/linux
)

#---
# specify the dependencies for this application
#
target_link_libraries(canopen-linux canopen-stack)
