#include "application_component.h"

namespace sometuke {

vector<unsigned char> FileUtils::ReadBytes(const string& relative_path) {
    ifstream io(FullPathFromRelativePath(relative_path));
    size_t size = io.seekg(0, ios::end).tellg();
    vector<unsigned char> buf(size);
    io.seekg(0, ios::beg).read(&buf[0], size);
    io.close();

    return buf;
}

string FileUtils::ReadString(const string &relative_path) {
    vector<unsigned char> buf = ReadBytes(relative_path);
    return string(buf.begin(), buf.end());
}

}
