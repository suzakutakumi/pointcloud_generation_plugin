cmake_minimum_required(VERSION 3.16)
project(pointcloud_generation_plugin)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++17
set(CMAKE_CXX_STANDARD ${CHOREONOID_CXX_STANDARD})
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

find_package(Choreonoid REQUIRED)
set(CMAKE_CXX_STANDARD ${CHOREONOID_CXX_STANDARD})

include_directories(${CHOREONOID_INCLUDE_DIRS})
link_directories(${CHOREONOID_LIBRARY_DIRS})

set(target CnoidGeneratePointCloudPlugin)
choreonoid_add_plugin(${target} src/pointcloud_gen_plugin.cpp)

target_link_libraries(${target} PUBLIC Choreonoid::CnoidBodyPlugin)

# add_library(${target} SHARED src/pointcloud_gen_plugin.cpp)

# ament_target_dependencies(${target} choreonoid)
# target_link_libraries(${target} ${CHOREONOID_BASE_LIBRARIES})

# install(TARGETS ${target} LIBRARY DESTINATION ${CHOREONOID_PLUGIN_DIR})

ament_package()