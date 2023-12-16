# BoringSSL.cmake

include(ExternalProject)

# 设置BoringSSL的URL和哈希值
set(BOSSL_URL "https://github.com/google/boringssl.git")
set(BOSSL_HASH "SHA256=...")  # 替换为实际的哈希值

# 添加ExternalProject
ExternalProject_Add(
    boringssl
    GIT_REPOSITORY ${BOSSL_URL}
    GIT_TAG master  # 或者你想使用的特定标签/分支
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

# 获取BoringSSL的include目录
ExternalProject_Get_Property(boringssl source_dir)
set(BOSSL_INCLUDE_DIR ${source_dir}/include)

# 获取BoringSSL的库目录
ExternalProject_Get_Property(boringssl binary_dir)
set(BOSSL_LIBRARY_DIR ${binary_dir})

# 创建BoringSSL库目标
add_library(BoringSSL INTERFACE)
target_include_directories(BoringSSL INTERFACE ${BOSSL_INCLUDE_DIR})
target_link_directories(BoringSSL INTERFACE ${BOSSL_LIBRARY_DIR})
target_link_libraries(BoringSSL INTERFACE ssl crypto)