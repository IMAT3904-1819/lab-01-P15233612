
#ifndef PROTOBUF_IO_H
#define PROTOBUF_IO_H

#include <iomanip>
#include <string>
#include <glog/logging.h>
#include <google/protobuf/message.h>

/**
 * @brief 从prototxt文本文件中读取Proto格式数据
 * @param filename prototxt文本文件的路径
 * @param proto 用来接收prototxt文件内容的对象
 * @return 是否成功
 */
bool ReadProtoFromTextFile(const char* filename, google::protobuf::Message* proto);

/**
 * @brief 从prototxt文本文件中读取Proto格式数据
 * @param filename prototxt文本文件的路径
 * @param proto 用来接收prototxt文件内容的对象
 * @return 是否成功
 */
inline bool ReadProtoFromTextFile(const std::string& filename, google::protobuf::Message* proto) {
  return ReadProtoFromTextFile(filename.c_str(), proto);
}

/**
 * @brief 从内存中读入Proto格式数据,内存中的数据为prototxt文本
 * @param data prototxt格式文本字符串
 * @param size 字符串的长度
 * @param proto 用来接收prototxt文件内容的对象
 * @return 是否成功
 */
bool ReadProtoFromInMemoryTextFile(const void* data, size_t size, google::protobuf::Message* proto);

/**
 * @brief 将Proto数据序列化为字符串,并将字符串写入文件
 * @param filename 用来储存prototxt文件的路径
 * @param proto 待写入prototxt文件的对象
 */
void WriteProtoToTextFile(const ::google::protobuf::Message& proto, const char* filename);

/**
 * @brief 将Proto数据序列化为字符串,并将字符串写入文件
 * @param filename 用来储存prototxt文件的路径
 * @param proto 待写入prototxt文件的对象
 */
inline void WriteProtoToTextFile(const ::google::protobuf::Message& proto, const std::string& filename) {
  WriteProtoToTextFile(proto, filename.c_str());
}

/**
 * @brief 从protobin二进制文件中读取Proto格式数据
 * @param filename 储存二进制格式序列化的文件路径
 * @param proto 用来接收数据的对象
 * @return 是否成功
 */
bool ReadProtoFromBinaryFile(const char* filename, google::protobuf::Message* proto);

/**
 * @brief 从protobin二进制文件中读取Proto格式数据
 * @param filename 储存二进制格式序列化的文件路径
 * @param proto 用来接收数据的对象
 * @return 是否成功
 */
inline bool ReadProtoFromBinaryFile(const std::string& filename, google::protobuf::Message* proto) {
  return ReadProtoFromBinaryFile(filename.c_str(), proto);
}

/**
 * @brief 从内存缓冲区中读取二进制序列化的数据,反序列化出Proto对象
 * @param data 序列化的二进制Proto数据
 * @param proto 用来接收数据的对象
 * @return 是否成功
 */
bool ReadProtoFromInMemoryBinaryFile(const void* data, size_t size, google::protobuf::Message* proto);

/**
 * @brief 将Proto数据序列化为二进制数据,并将字符串写入文件
 * @param filename 用来储存序列化后二进制数据的文件路径
 * @param proto 待写入文件的对象
 */
void WriteProtoToBinaryFile(const google::protobuf::Message& proto, const char* filename);

/**
 * @brief 将Proto数据序列化为二进制数据,并将字符串写入文件
 * @param filename 用来储存序列化后二进制数据的文件路径
 * @param proto 待写入文件的对象
 */
inline void WriteProtoToBinaryFile(
        const google::protobuf::Message& proto, const std::string& filename) {
  WriteProtoToBinaryFile(proto, filename.c_str());
}


#endif   // PROTOBUF_IO_H

