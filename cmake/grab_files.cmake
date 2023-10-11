set(LIB_SRC
        ${PROJECT_SOURCE_DIR}/src/trt_deploy.cpp
        ${PROJECT_SOURCE_DIR}/src/util.cpp
        ${PROJECT_SOURCE_DIR}/src/config.cpp
        ${PROJECT_SOURCE_DIR}/src/preprocessor.cpp
        ${PROJECT_SOURCE_DIR}/src/preprocess_ops.cpp
        ${PROJECT_SOURCE_DIR}/src/trt_deployresult.cpp
        ${PROJECT_SOURCE_DIR}/src/postprocessor.cpp
        ${PROJECT_SOURCE_DIR}/src/model.cpp
        )

set(LIB_HEADER
        ${PROJECT_SOURCE_DIR}/src/trt_deploy.h
        ${PROJECT_SOURCE_DIR}/src/util.h
        ${PROJECT_SOURCE_DIR}/src/macro.h
        ${PROJECT_SOURCE_DIR}/src/cmdline.h
        ${PROJECT_SOURCE_DIR}/src/config.h
        ${PROJECT_SOURCE_DIR}/src/preprocessor.h
        ${PROJECT_SOURCE_DIR}/src/preprocess_util.hpp
        ${PROJECT_SOURCE_DIR}/src/preprocess_ops.h
        ${PROJECT_SOURCE_DIR}/src/trt_deployresult.h
        ${PROJECT_SOURCE_DIR}/src/postprocessor.h
        ${PROJECT_SOURCE_DIR}/src/model.h
        )

set(LIB_MAIN
        ../test/main.cpp
        )


