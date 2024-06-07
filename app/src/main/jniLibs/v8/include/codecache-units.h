/**
 * Copyright (C) 2017 Alibaba Group Holding Limited. All Rights Reserved.
 */
#ifndef YUNOS_INCLUDE_CODECACHE_UNITS_H
#define YUNOS_INCLUDE_CODECACHE_UNITS_H

namespace yunos {

class CodeCacheUnits {
 public:
  static bool IsGenerateCodeCacheNeeded(const char* appRootPath);
  static bool GenerateCodeCache(const char* appRootPath);
};
}
#endif // YUNOS_INCLUDE_CODECACHE_UNITS_H
