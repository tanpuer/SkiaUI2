plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
}

android {
    namespace = "com.temple.skiaui"
    compileSdk = 35

    defaultConfig {
        applicationId = "com.temple.skiaui"
        minSdk = 29
        targetSdk = 35
        versionCode = 1
        versionName = "1.0"

        externalNativeBuild {
            cmake {
                cppFlags += "-std=c++17"
                abiFilters += "arm64-v8a"
                abiFilters += "x86_64"
                arguments += "-DANDROID_TOOLCHAIN=clang"
                arguments += "-DANDROID_STL=c++_shared"
            }
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    androidResources {
        noCompress += "filamat"
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }
    kotlinOptions {
        jvmTarget = "1.8"
    }
    externalNativeBuild {
        cmake {
            path = file("CMakeLists.txt")
            version = "3.22.1"
        }
    }
    buildFeatures {
        compose = true
    }
}

dependencies {
    implementation("androidx.appcompat:appcompat:1.7.0")
    implementation("androidx.constraintlayout:constraintlayout:2.2.1")
    implementation("org.java-websocket:Java-WebSocket:1.5.6")
    implementation("androidx.media3:media3-exoplayer:1.5.1")
    implementation("androidx.compose.runtime:runtime:1.7.8")
    implementation("androidx.compose.compiler:compiler:1.5.15")
    implementation("androidx.compose.foundation:foundation-android:1.7.8")
    implementation("com.google.android.filament:filament-android:1.56.0")
}