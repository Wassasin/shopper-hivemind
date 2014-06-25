project(shopper-hivemind)

file(GLOB_RECURSE shopper-hivemind_HEADERS ./*.h)
file(GLOB_RECURSE shopper-hivemind_SOURCES ./*.cpp)

find_package(Boost COMPONENTS system filesystem regex program_options iostreams REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

find_package(msgpack REQUIRED)
include_directories(SYSTEM ${msgpack_INCLUDE_DIRS})

find_package(svm REQUIRED)
include_directories(SYSTEM ${svm_INCLUDE_DIRS})


include(ExternalProject)

add_custom_target(
   liblinear
   COMMAND make lib
   WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/../contrib/liblinear"
)

set(linear_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/../contrib/liblinear")
set(linear_LIBRARIES "${PROJECT_SOURCE_DIR}/../contrib/liblinear/liblinear.so.1")

include_directories(SYSTEM ${linear_INCLUDE_DIRS})

find_package(Qt5Core REQUIRED)
add_definitions(${QT5Core_DEFINITIONS})

add_executable(shopper-hivemind ${shopper-hivemind_SOURCES} ${shopper-hivemind_HEADERS})
qt5_use_modules(shopper-hivemind Core)
include_directories(.)
target_link_libraries(shopper-hivemind ${Boost_LIBRARIES} ${msgpack_LIBRARIES} ${QT_LIBRARIES} ${svm_LIBRARIES} ${linear_LIBRARIES})

add_dependencies(shopper-hivemind liblinear)
