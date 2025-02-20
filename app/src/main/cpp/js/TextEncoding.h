#pragma once

#include "string"
#include "memory"
#include "SkiaUIContext.h"

namespace HYSkiaUI {

class TextEncoding {

public:

    TextEncoding();

    TextEncoding(TextEncoding &file) = delete;

    void operator=(TextEncoding &file) = delete;

    const char* encode(std::string& value);

};

}
