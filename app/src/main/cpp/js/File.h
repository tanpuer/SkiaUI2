#pragma once

#include "string"
#include "memory"
#include "SkiaUIContext.h"

namespace HYSkiaUI {

class File {

public:

    File() = delete;

    File(File &file) = delete;

    void operator=(File &file) = delete;

    File(std::string path, std::shared_ptr<SkiaUIContext> &context);

    bool exist();

    std::string read();

private:

    std::string path;

    std::shared_ptr<SkiaUIContext> context = nullptr;

};

}
