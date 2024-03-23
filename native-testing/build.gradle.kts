plugins {
    alias(libs.plugins.androidLibrary)
    alias(libs.plugins.kotlinAndroid)
}

android {
    namespace = "com.mistersomov.native_testing"
    compileSdk = 34

    defaultConfig {
        minSdk = 28

        externalNativeBuild {
            cmake {
                targets += "native_testing"
                arguments(
                    "-DNDK_HELPER_PATH=${project(":common:ndk_helper").projectDir}",
                )
            }
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = true
        }
    }
    externalNativeBuild {
        cmake {
            path("CMakeLists.txt")
            version = "3.22.1"
            ndkVersion = "26.1.10909125"
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }
    kotlinOptions {
        jvmTarget = "1.8"
    }
    buildFeatures {
        prefab = true
    }
}

dependencies {
    api(libs.game.activity)

    // tests
    implementation(libs.androidx.junit.gtest)
    implementation(libs.googletest)
}