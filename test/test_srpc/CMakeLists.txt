# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
aux_source_directory(../proto DIR_SRCS)

# 设置头文件
include_directories(../../3rd/protobuf/include)
include_directories(../../inc)
include_directories(../network/inc)
include_directories(../proto)

# 设置输出目录
set(EXECUTABLE_OUTPUT_PATH ${BIN_OUTPUT_PATH})
set(LINK_LIBS ${BIN_OUTPUT_PATH}/libsrpc.a ${PROJECT_SOURCE_DIR}/3rd/protobuf/lib/linux/libprotobuf.a pthread)
# 设置链接选项
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath=./ ")
# 生成test
add_executable(TestSRPC ${DIR_SRCS})
# 链接库
target_link_libraries(TestSRPC ${LINK_LIBS})