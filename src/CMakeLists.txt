project(shopper-hivemind)

file(GLOB_RECURSE shopper-hivemind_HEADERS ./*.h)
file(GLOB_RECURSE shopper-hivemind_SOURCES ./*.cpp)

find_package(Boost COMPONENTS system filesystem regex program_options iostreams REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

find_package(msgpack REQUIRED)
include_directories(SYSTEM ${msgpack_INCLUDE_DIRS})

find_package(svm REQUIRED)
include_directories(SYSTEM ${svm_INCLUDE_DIRS})

find_package(linear REQUIRED)
include_directories(SYSTEM ${linear_INCLUDE_DIRS})

find_package(Qt5Core REQUIRED)
add_definitions(${QT5Core_DEFINITIONS})

add_executable(shopper-hivemind ${shopper-hivemind_SOURCES} ${shopper-hivemind_HEADERS})
qt5_use_modules(shopper-hivemind Core)
include_directories(.)
target_link_libraries(shopper-hivemind ${Boost_LIBRARIES} ${msgpack_LIBRARIES} ${QT_LIBRARIES} ${svm_LIBRARIES} ${linear_LIBRARIES})
