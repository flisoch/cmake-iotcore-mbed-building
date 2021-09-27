# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")
set(APP_TARGET switch_example)
set(CMAKE_BUILD_PATH "cmake_build/NUCLEO_F446RE/develop/GCC_ARM")
include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

add_subdirectory(${MBED_PATH})

add_executable(${APP_TARGET}
    main.cpp
)

# возможно можно убрать, когда линковку mbed-os через iot-core делаю
target_link_libraries(${APP_TARGET} PUBLIC mbed-os)

mbed_set_post_build(${APP_TARGET})

option(VERBOSE_BUILD "Have a verbose build process")
if(VERBOSE_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()



# copy from generated flags of esp32
#-------------------------------------------------------
set(CONFIG_STDK_IOT_CORE "y")
set(CONFIG_STDK_IOT_CORE_EASYSETUP_DISCOVERY_SSID "y")
set(CONFIG_STDK_IOT_CORE_EASYSETUP_HTTP "y")
set(CONFIG_STDK_IOT_CORE_EASYSETUP_HTTP_USE_SOCKET_API "y")
set(CONFIG_STDK_IOT_CORE_EASYSETUP_X509 "")
set(CONFIG_STDK_IOT_CORE_EASYSETUP_LOG_SUPPORT_NO_USE_LOGFILE "")
set(CONFIG_STDK_IOT_CORE_LOG_LEVEL_ERROR "y")
set(CONFIG_STDK_IOT_CORE_LOG_LEVEL_WARN "y")
set(CONFIG_STDK_IOT_CORE_LOG_LEVEL_INFO "y")
set(CONFIG_STDK_IOT_CORE_LOG_LEVEL_DEBUG "")
set(CONFIG_STDK_IOT_CORE_SUPPORT_STNV_PARTITION "")
set(CONFIG_STDK_IOT_CORE_LOG_FILE "y")
set(CONFIG_STDK_IOT_CORE_LOG_FILE_RAM_ONLY "y")
set(CONFIG_STDK_IOT_CORE_LOG_FILE_FLASH_WITH_RAM "")
set(CONFIG_STDK_IOT_CORE_LOG_FILE_RAM_BUF_SIZE "8192")
set(CONFIG_STDK_IOT_CORE_BSP_SUPPORT_ESP8266 "")
set(CONFIG_STDK_IOT_CORE_BSP_SUPPORT_ESP32 "")
set(CONFIG_STDK_IOT_CORE_BSP_SUPPORT_ESP32S2 "")
set(CONFIG_STDK_IOT_CORE_BSP_SUPPORT_NUCLEOF410RB "y")
set(CONFIG_STDK_IOT_CORE_OS_SUPPORT_FREERTOS "")
set(CONFIG_STDK_IOT_CORE_OS_SUPPORT_TIZENRT "")
set(CONFIG_STDK_IOT_CORE_OS_SUPPORT_POSIX "")
set(CONFIG_STDK_IOT_CORE_OS_SUPPORT_MBEDOS "y")
set(CONFIG_STDK_DEBUG_MEMORY_CHECK "")
set(CONFIG_STDK_IOT_CORE_USE_MBEDTLS "y")
set(CONFIG_STDK_IOT_CORE_CRYPTO_SUPPORT_RSA "")
set(CONFIG_STDK_IOT_CORE_CRYPTO_SUPPORT_ECDSA "")
set(CONFIG_STDK_IOT_CORE_CRYPTO_SUPPORT_ED25519 "y")
set(CONFIG_STDK_IOT_CORE_SECURITY_BACKEND_SOFTWARE "y")
set(CONFIG_STDK_IOT_CORE_SECURITY_BACKEND_HARDWARE "")
set(CONFIG_STDK_IOT_CORE_FS_SW_ENCRYPTION "")
set(CONFIG_STDK_IOT_CORE_FS_HW_ENCRYPTION "y")
set(CONFIG_STDK_IOT_CORE_NET_MBEDTLS "y")
set(CONFIG_STDK_IOT_CORE_NET_OPENSSL "")
#-------------------------------------------------------
set(STDK_IOT_CORE_USE_DEFINED_CONFIG "y")
set(CONFIG_STDK_IOT_CORE_EASYSETUP_HTTP_USE_SOCKET_API "y")

set(STDK_CORE_PATH "/home/flisoch/git-repos/st-device-sdk-c-ref/iot-core")
set(STDK_INCLUDE_PATH "/home/flisoch/git-repos/st-device-sdk-c-ref/iot-core/src/include")

#------
MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        if(NOT "${dir_path}" MATCHES ".*/targets/.*")
            SET(dir_list ${dir_list} ${dir_path})
            # message(${dir_path})
        endif()
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
    # message(${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_dir_list)
list(LENGTH header_dir_list header_dir_list_count)
message(STATUS "[INFO] Found ${header_dir_list_count} header directories.")


set(STDK_MBED_INCLUDE 
        "mbed-os/connectivity/lwipstack/lwip/src/include/"
)


add_subdirectory(${STDK_CORE_PATH} iotcore)
target_link_libraries(iotcore PUBLIC mbed-os)
target_link_libraries(${APP_TARGET} PUBLIC iotcore)

target_include_directories(iotcore PUBLIC .)



# load libsodium
# -------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(Sodium
    GIT_REPOSITORY https://github.com/robinlinden/libsodium-cmake.git
)
set(SODIUM_DISABLE_TESTS ON)
FetchContent_MakeAvailable(Sodium)

target_link_libraries(${APP_TARGET} PUBLIC sodium
)
set(SODIUM_INCLUDE_PATH ${CMAKE_BUILD_PATH}/_deps/sodium-src/libsodium/src/libsodium/include)

# load cJSON
# -------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(cJSON
    GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
)
FetchContent_MakeAvailable(cJSON)

target_link_libraries(${APP_TARGET} PUBLIC cjson
)
set(CJSON_INCLUDE_PATH ${CMAKE_BUILD_PATH}/_deps/cjson-src/)


# include mbed header files paths to iot-core(?!)
#--------------------------------------------
target_include_directories(iotcore PUBLIC 
        ${STDK_MBED_INCLUDE}
        ${SODIUM_INCLUDE_PATH}
        ${CJSON_INCLUDE_PATH}
        ${header_dir_list}
)

