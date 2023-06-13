set(SOURCE_DIR ../../..)

set(TARGET_NAME setpoint-slave)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME}
  PRIVATE
    # application
    app.cpp
    spec.cpp
    main.cpp
    # config
    ${SOURCE_DIR}/config/hw.c
    # driver
    ${SOURCE_DIR}/driver/linux/drv_can_socketcan.cpp
    ${SOURCE_DIR}/driver/linux/drv_nvm_file.cpp
    ${SOURCE_DIR}/driver/linux/drv_timer_cpp.cpp
)

target_include_directories(${TARGET_NAME}
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${SOURCE_DIR}/config
    ${SOURCE_DIR}/driver/linux
)

target_link_libraries(${TARGET_NAME} canopen-stack)
