/**
 * Copyright (C) 2017 Alibaba Group Holding Limited. All Rights Reserved.
 */
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef YUNOS_INCLUDE_MKJSO_UNITS_H
#define YUNOS_INCLUDE_MKJSO_UNITS_H
#include <string>
#include <vector>

namespace yunos {

class MkJsoUnits {
 public:
  /**
  * Generate the app's jso file.
  *
  * Parameters:
  *   appRootPath:  Need to generate a jso file application directory.
  */
  static bool GenerateJso(const char* appRootPath,
                          bool skip_if_aot_exists = true);

  /**
  * Clean the jso files of framework and all apps.
  *
  * return:  return true if success, otherwise false.
  */
  static bool CleanAllJso();


  /**
  * Find all files with specific suffix in the specified directory "path"
  * and save the full path name of found files into vector fileList.
  *
  * Parameters:
  *   path:     Directory to search.
  *   suffix:   File suffix to search for.
  *   depth:    Search the N layers of directories only if it is N, depth=-1 is
  *             search the total for a directory.
  *   fileList: List of search results.
  *   skipList: List of directories skipped when searching.
  */
  static void FindAllSpecificFiles(const std::string& path,
                                         const std::string& suffix,
                                         int depth,
                                         std::vector<std::string>& fileList,
                                         std::vector<const char*> skipList={});
}; // MkJsoUnits
} //namespace
#endif  // YUNOS_INCLUDE_MKJSO_UNITS_H
