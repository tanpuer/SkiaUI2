cmake_minimum_required(VERSION 3.22.1)
add_compile_options(-Wconversion -Wsign-conversion)

project("skiaui")

set(JNI_LIBS_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs)

include_directories(
        ${JNI_LIBS_DIR}/skia
        ${JNI_LIBS_DIR}/skia/include
        ${JNI_LIBS_DIR}/skia/modules
        ${JNI_LIBS_DIR}/skia/src

        src/main/cpp
        src/main/cpp/base
        src/main/cpp/bitmap
        src/main/cpp/filter
        src/main/cpp/yoga
        src/main/cpp/yoga/event
        src/main/cpp/widget
        src/main/cpp/test
        src/main/cpp/touch
        src/main/cpp/anim
        src/main/cpp/app
        src/main/cpp/plugin
        src/main/cpp/page
        src/main/cpp/js
        src/main/cpp/inspect
        src/main/cpp/resources
        src/main/cpp/custom
        src/main/cpp/compose
)

link_directories(
        ${JNI_LIBS_DIR}/${ANDROID_ABI}
)

# Import prebuilt SKIA libraries

add_library(skia STATIC IMPORTED)
set_property(TARGET skia PROPERTY IMPORTED_LOCATION "${JNI_LIBS_DIR}/${ANDROID_ABI}/libskia.a")

add_library(svg STATIC IMPORTED)
set_property(TARGET svg PROPERTY IMPORTED_LOCATION "${JNI_LIBS_DIR}/${ANDROID_ABI}/libsvg.a")

add_library(skshaper STATIC IMPORTED)
set_property(TARGET skshaper PROPERTY IMPORTED_LOCATION "${JNI_LIBS_DIR}/${ANDROID_ABI}/libskshaper.a")

add_library(skparagraph STATIC IMPORTED)
set_property(TARGET skparagraph PROPERTY IMPORTED_LOCATION "${JNI_LIBS_DIR}/${ANDROID_ABI}/libskparagraph.a")

add_library(skunicode_core STATIC IMPORTED)
set_property(TARGET skunicode_core PROPERTY IMPORTED_LOCATION "${JNI_LIBS_DIR}/${ANDROID_ABI}/libskunicode_core.a")

add_library(skunicode_icu STATIC IMPORTED)
set_property(TARGET skunicode_icu PROPERTY IMPORTED_LOCATION "${JNI_LIBS_DIR}/${ANDROID_ABI}/libskunicode_icu.a")

add_library(skottie STATIC IMPORTED)
set_property(TARGET skottie PROPERTY IMPORTED_LOCATION "${JNI_LIBS_DIR}/${ANDROID_ABI}/libskottie.a")

add_library(sksg STATIC IMPORTED)
set_property(TARGET sksg PROPERTY IMPORTED_LOCATION "${JNI_LIBS_DIR}/${ANDROID_ABI}/libsksg.a")

##v8-start
add_library(v8_libbase SHARED IMPORTED)
set_target_properties(v8_libbase
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libv8_libbase.cr.so)
add_library(v8_libplatform SHARED IMPORTED)
set_target_properties(v8_libplatform
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libv8_libplatform.cr.so)
add_library(v8 SHARED IMPORTED)
set_target_properties(v8
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libv8.cr.so)
add_library(icui18n SHARED IMPORTED)
set_target_properties(icui18n
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libicui18n.cr.so)
add_library(icuuc SHARED IMPORTED)
set_target_properties(icuuc
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libicuuc.cr.so)
add_library(chrome_zlib SHARED IMPORTED)
set_target_properties(chrome_zlib
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libchrome_zlib.cr.so)
include_directories(${JNI_LIBS_DIR}/v8/include)
##v8-end

aux_source_directory(src/main/cpp srcs)
aux_source_directory(src/main/cpp/base srcs)
aux_source_directory(src/main/cpp/bitmap srcs)
aux_source_directory(src/main/cpp/filter srcs)
aux_source_directory(src/main/cpp/yoga srcs)
aux_source_directory(src/main/cpp/yoga/event srcs)
aux_source_directory(src/main/cpp/widget srcs)
aux_source_directory(src/main/cpp/test srcs)
aux_source_directory(src/main/cpp/touch srcs)
aux_source_directory(src/main/cpp/anim srcs)
aux_source_directory(src/main/cpp/app srcs)
aux_source_directory(src/main/cpp/plugin srcs)
aux_source_directory(src/main/cpp/page srcs)
aux_source_directory(src/main/cpp/js srcs)
aux_source_directory(src/main/cpp/inspect srcs)
aux_source_directory(src/main/cpp/resources srcs)
aux_source_directory(src/main/cpp/custom srcs)
aux_source_directory(src/main/cpp/compose srcs)

add_library(skiaui SHARED ${srcs})

find_library(log-lib log)
find_library(z-lib z)
find_library(android-lib android)

target_link_libraries(skiaui

        skia
        svg
        skshaper
        skparagraph
        skunicode_core
        skunicode_icu
        skottie
        sksg

        v8_libbase v8_libplatform icui18n icuuc chrome_zlib v8

        android EGL GLESv3 jnigraphics
        ${LINK_LIB_LIST} ${log-lib} ${z-lib} ${android-lib})