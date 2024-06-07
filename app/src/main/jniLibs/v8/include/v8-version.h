// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_INCLUDE_VERSION_H_  // V8_VERSION_H_ conflicts with src/version.h
#define V8_INCLUDE_VERSION_H_

// These macros define the version number for the current version.
// NOTE these macros are used by some of the tool scripts and the build
// system so their names cannot be changed without changing the scripts.
#define V8_MAJOR_VERSION 8
#define V8_MINOR_VERSION 1
#define V8_BUILD_NUMBER 307
#define V8_PATCH_LEVEL 31

// <YUNOS> changed: begin
// Version for AliOS
#define V8_ALIOS_MAJOR_VERSION 3
#define V8_ALIOS_MINOR_VERSION 0
#define V8_ALIOS_PATCH_LEVEL 0
#define V8_ALIOS_PROJECT_UPDATE 0

// Version for XTS
#define V8_XTS_MAJOR_VERSION 3
#define V8_XTS_MINOR_VERSION 1
#define V8_XTS_PATCH_LEVEL 0

// Version for JSAOT
#define V8_JSAOT_MAJOR_VERSION 2
#define V8_JSAOT_MINOR_VERSION 2
#define V8_JSAOT_PATCH_LEVEL 0

// Version for VEX
#define V8_VEX_MAJOR_VERSION 1
#define V8_VEX_MINOR_VERSION 0
#define V8_VEX_PATCH_LEVEL 0
// <YUNOS> changed: end

// Use 1 for candidates and 0 otherwise.
// (Boolean macro values are not supported by all preprocessors.)
#define V8_IS_CANDIDATE_VERSION 0

#endif  // V8_INCLUDE_VERSION_H_
