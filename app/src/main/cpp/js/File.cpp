#include "File.h"
#include "native_log.h"

namespace HYSkiaUI {

File::File(std::string path, std::shared_ptr<SkiaUIContext> &context) : path(std::move(path)) {
    this->context = context;
    ALOGD("File:: new File %s", path.c_str())
}

bool File::exist() {
    if (context == nullptr) {
        return false;
    }
    ALOGD("File:: exist")
    return context->getAssetManager()->exist(path.c_str());
}

std::string File::read() {
    if (context == nullptr) {
        return "";
    }
    ALOGD("File:: read %s", path.c_str())
    return context->getAssetManager()->readFile(path.c_str());
}

}
