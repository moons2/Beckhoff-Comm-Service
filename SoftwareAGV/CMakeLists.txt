cmake_minimum_required(VERSION 2.8)

# require C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Opencv setup
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

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

#Em target_link_libraries ordem da declacao dos arquivos importa e deve ser respeitada.
#Os arquivos mais dependentes são declarados primeiro, seguido de suas dependências.
target_link_libraries( AGVbeckhoff ${OPENCV_LIBRARIES} MaintenanceSoftware Facade_Beckhoff_Communication_Service Beckhoff_Communication_Service libethercat pthread m)
