#include "TextEncoding.h"

namespace HYSkiaUI {

TextEncoding::TextEncoding() {

}

const char *TextEncoding::encode(std::string &value) {
    return value.c_str();
}

}
