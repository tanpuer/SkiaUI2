cmake_minimum_required(VERSION 3.22.1)

project("skiaui")

set(JNI_LIBS_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs)

include_directories(
        ${JNI_LIBS_DIR}/skia
        ${JNI_LIBS_DIR}/skia/include
        ${JNI_LIBS_DIR}/skia/modules
        ${JNI_LIBS_DIR}/skia/src

        src/main/cpp
        src/main/cpp/base
        src/main/cpp/fliter
        src/main/cpp/yoga
        src/main/cpp/yoga/event
        src/main/cpp/widget
        src/main/cpp/test
        src/main/cpp/touch
        src/main/cpp/anim
        src/main/cpp/app
        src/main/cpp/plugin
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

add_library(skunicode STATIC IMPORTED)
set_property(TARGET skunicode PROPERTY IMPORTED_LOCATION "${JNI_LIBS_DIR}/${ANDROID_ABI}/libskunicode.a")

add_library(skottie STATIC IMPORTED)
set_property(TARGET skottie PROPERTY IMPORTED_LOCATION "${JNI_LIBS_DIR}/${ANDROID_ABI}/libskottie.a")

add_library(sksg STATIC IMPORTED)
set_property(TARGET sksg PROPERTY IMPORTED_LOCATION "${JNI_LIBS_DIR}/${ANDROID_ABI}/libsksg.a")

aux_source_directory(src/main/cpp srcs)
aux_source_directory(src/main/cpp/base srcs)
aux_source_directory(src/main/cpp/fliter srcs)
aux_source_directory(src/main/cpp/yoga srcs)
aux_source_directory(src/main/cpp/yoga/event srcs)
aux_source_directory(src/main/cpp/widget srcs)
aux_source_directory(src/main/cpp/test srcs)
aux_source_directory(src/main/cpp/touch srcs)
aux_source_directory(src/main/cpp/anim srcs)
aux_source_directory(src/main/cpp/app srcs)
aux_source_directory(src/main/cpp/plugin srcs)

add_library(skiaui SHARED ${srcs})

find_library(log-lib log)
find_library(z-lib z)
find_library(android-lib android)

target_link_libraries(skiaui

        skia
        svg
        skshaper
        skparagraph
        skunicode
        skottie
        sksg

        android EGL GLESv3 jnigraphics
        ${LINK_LIB_LIST} ${log-lib} ${z-lib} ${android-lib})