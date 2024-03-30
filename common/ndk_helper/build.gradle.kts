plugins {
    alias(libs.plugins.androidLibrary)
    alias(libs.plugins.kotlinAndroid)
}

android {
    namespace = "com.mistersomov.ndk_helper"
    compileSdk = 34

    defaultConfig {
        minSdk = 28

        externalNativeBuild {
            cmake {
                targets += "ndk_helper"
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
    externalNativeBuild {
        cmake {
            ndkVersion = "26.1.10909125"
            path("CMakeLists.txt")
            version = "3.22.1"
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
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.test.ext)
}