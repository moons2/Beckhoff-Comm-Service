cmake_minimum_required(VERSION 2.8)

# require C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#project properies
project( AGVbeckhoff )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
add_executable( AGVbeckhoff src/SuperMain/main.cpp )

#include archives from services and softwares
#SOFTWARES
#MaintenanceSoftware
add_library(MaintenanceSoftware src/Softwares/MaintenanceSoftware/MaintenanceSoftware.cpp)

#SERVICES
#Communication_Monitoring
add_library(Facade_Beckhoff_Communication_Service src/Services/BeckhoffCommunicationService/Facade_Beckhoff_Communication_Service.cpp)
add_library(Beckhoff_Communication_Service src/Services/BeckhoffCommunicationService/Beckhoff_Communication_Service.cpp)
#include static library ethercat.a for beckhoff communication services
ADD_LIBRARY(libethercat STATIC IMPORTED)
SET_TARGET_PROPERTIES(libethercat PROPERTIES IMPORTED_LOCATION libethercat/ethercat.a)

#configuring OpenCV
if(OCVV EQUAL 2_4)
    message(STATUS "OpenCV version required: ${OCVV}")
    SET(CMAKE_PREFIX_PATH /usr/local/oecore-opencv${OCVV}/sysroots/armv7at2hf-vfp-neon-angstrom-linux-gnueabi)
elseif(OCVV EQUAL 3_1)
    message(STATUS "OpenCV version required: ${OCVV}")
    SET(CMAKE_PREFIX_PATH /usr/local/oecore-x86_64/sysroots/armv7at2hf-neon-angstrom-linux-gnueabi)
else()
    message(FATAL_ERROR "OpenCV version needs to be passed. Make sure it matches your SDK version. Use -DOCVV=<version>, currently supported 2_4 and 3_1. E.g. -DOCVV=3_1")
endif()
SET(OpenCV_DIR ${CMAKE_PREFIX_PATH}/usr/lib/cmake/OpenCV)

find_package( OpenCV REQUIRED )
include_directories( ${OPENCV_INCLUDE_DIRS} )

#Em target_link_libraries ordem da declacao dos arquivos importa e deve ser respeitada.
#Os arquivos mais dependentes são declarados primeiro, seguido de suas dependências.
target_link_libraries( AGVbeckhoff ${OPENCV_LIBRARIES} MaintenanceSoftware Facade_Beckhoff_Communication_Service Beckhoff_Communication_Service libethercat pthread m)
