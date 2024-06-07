// Copyright 2017 the Alibaba YunOS project.  All rights reserved.
#ifndef INCLUDE_V8_JSAOT_H_
#define INCLUDE_V8_JSAOT_H_

#define V8_JSCACHE_PATH "/data/js-cache/"
#define V8_FRAMEWORK_PATH "/usr/framework/"
#define V8_FRAMEWORK_JSO_PATH V8_JSCACHE_PATH "framework.jso"
#define OFFLINE_V8_FRAMEWORK_JSO_PATH V8_FRAMEWORK_PATH "framework.jso"
#define V8_JS_SUFFIX ".js"
#define V8_VEX_SUFFIX ".vex"
#define V8_JSO_SUFFIX ".jso"
#define V8_JSO_LIST_NAME "jso_file.list"
// The root directory of all applications
#define V8_APPS_PATH "/opt/app/"

#define LOAD_FAILED -1
#define LOAD_FAILED_REMOVE_JSO_FILE -2

const int MB = 1024 * 1024;

// Leave enough virtual memory for Seed.
#define JSORANGE_SIZE (128 * sizeof(void*) / 4)

#include <stdint.h>
// The same value with Name::kHashBitMask, Name::kNoFHashBitFields and
// Name::kHashShift
// Shift constant retrieving hash code from hash field.
static const int kJsoNofHashBitFields = 2;
static const int kJsoHashShift = kJsoNofHashBitFields;
static const uint32_t kJsoHashBitMask = 0xffffffffu >> kJsoHashShift;

// The content of JsoFileHeaderFake have to keep consistent with JsoFileHeader.
struct JsoFileHeaderFake {
  const uint8_t magic_number[4] = {'J', 'S', 'O', '\0'};
  uint32_t num_unbound_scripts = 0;
  uint32_t num_internalized_strings = 0;
  uint32_t num_internalized_strings_ref_lists = 0;
  uint32_t num_normal_object_pages = 0;
  uint32_t num_large_object_pages = 0;
  uintptr_t aot_cache_address = 0;
  size_t file_size = 0;
  uint32_t hash_seed = 0;
  uintptr_t heap_checksum = 0;
  uintptr_t jso_range_start_size = 0;
};

#endif // INCLUDE_V8_JSAOT_H_
