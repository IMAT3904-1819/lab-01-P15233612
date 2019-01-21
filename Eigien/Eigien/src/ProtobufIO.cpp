#include <fcntl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <cstdint>
#include <fstream>
#include <glog/logging.h>
#include <io.h>

const int kProtoReadBytesLimit = INT_MAX;  // Max size of 2 GB minus 1 byte.

using google::protobuf::io::FileInputStream;
using google::protobuf::io::FileOutputStream;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::CodedInputStream;
using google::protobuf::io::ZeroCopyOutputStream;
using google::protobuf::io::CodedOutputStream;
using google::protobuf::io::ArrayInputStream;
using google::protobuf::Message;
using namespace std;

bool ReadProtoFromTextFile(const char* filename, Message* proto) {
    int fd = _open(filename, O_RDONLY);
    CHECK_NE(fd, -1) << "File not found: " << filename;
    FileInputStream* input = new FileInputStream(fd);
    bool success = google::protobuf::TextFormat::Parse(input, proto);
    delete input;
    _close(fd);
    return success;
}

bool ReadProtoFromInMemoryTextFile(const void* data, size_t size, Message* proto)
{
    ArrayInputStream* input = new ArrayInputStream(data, size);
    bool success = google::protobuf::TextFormat::Parse(input, proto);
    delete input;
    return success;
}

void WriteProtoToTextFile(const Message& proto, const char* filename) {
    int fd = _open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    FileOutputStream* output = new FileOutputStream(fd);
    CHECK(google::protobuf::TextFormat::Print(proto, output));
    delete output;
    _close(fd);
}

bool ReadProtoFromBinaryFile(const char* filename, Message* proto) {
#if defined (_MSC_VER)  // for MSC compiler binary flag needs to be specified
    int fd = _open(filename, O_RDONLY | O_BINARY);
#else
    int fd = open(filename, O_RDONLY);
#endif
    CHECK_NE(fd, -1) << "File not found: " << filename;
    ZeroCopyInputStream* raw_input = new FileInputStream(fd);
    CodedInputStream* coded_input = new CodedInputStream(raw_input);
    coded_input->SetTotalBytesLimit(kProtoReadBytesLimit, 536870912);

    bool success = proto->ParseFromCodedStream(coded_input);

    delete coded_input;
    delete raw_input;
    _close(fd);
    return success;
}

bool ReadProtoFromInMemoryBinaryFile(const void* data, size_t size, Message* proto)
{
    ArrayInputStream* raw_input = new ArrayInputStream(data, size);
    CodedInputStream* coded_input = new CodedInputStream(raw_input);
    coded_input->SetTotalBytesLimit(kProtoReadBytesLimit, 536870912);

    bool success = proto->ParseFromCodedStream(coded_input);

    delete coded_input;
    delete raw_input;
    return success;
}

void WriteProtoToBinaryFile(const Message& proto, const char* filename) {
    fstream output(filename, ios::out | ios::trunc | ios::binary);
    CHECK(proto.SerializeToOstream(&output));
}
