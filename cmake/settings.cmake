## Author: Guangfu WANG.
## Date: 2023-08-20.
#set cpp version used in this project.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#this is equivalently to -fPIC in cxx_flags.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#define options for custom build targets.
option(GEN_TEST "Build fight test program." ON)
option(PREPROCESS_GPU "Use GPU version of preprocessing pipeline" ON)
set(MODEL_INPUT_NAME "im_shape image scale_factor" CACHE STRING "Input layer name for tensorrt deploy.")
set(MODEL_OUTPUT_NAMES "dets num_dets" CACHE STRING "Output layer names for tensorrt deploy, seperated with comma or colon")
set(DEPLOY_MODEL "../models/smoke.engine" CACHE STRING "Used deploy AI model file (/path/to/*.engine)")

# generate config.h in src folder.
configure_file(
        "${PROJECT_SOURCE_DIR}/src/macro.h.in"
        "${PROJECT_SOURCE_DIR}/src/macro.h"
        @ONLY
)

set(DEPLOY_LIB_NAME "smoke_detection")
set(DEPLOY_MAIN_NAME "smoke_detection_main")

set(CMAKE_INSTALL_RPATH "\$ORIGIN")
set(CMAKE_INSTALL_PREFIX "install")
add_link_options("-Wl,--as-needed")



