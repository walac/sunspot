add_definitions (-D_GNU_SOURCE)

find_path (OPENCV_INCLUDE_DIR "cv.h" PATH_SUFFIXES opencv)
include_directories(${OPENCV_INCLUDE_DIR})
set (OPENCV_LIBS cv highgui cvaux ml cxcore)

add_executable (sunspot detect.cpp blob.cpp main.cpp mag.cpp group.cpp)

target_link_libraries (sunspot ${OPENCV_LIBS})
