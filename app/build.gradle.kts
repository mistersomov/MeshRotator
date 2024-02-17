plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
}

val appNameDev = "MeshRotator Dev"
val appNameProd = "MeshRotator"

android {
    namespace = "com.mistersomov.meshrotator"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.mistersomov.meshrotator"
        minSdk = 28
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        externalNativeBuild {
            cmake {
                cppFlags += "-std=c++11 -frtti -Wall -Werror"
                arguments(
                    "-DNDK_HELPER_PATH=${project(":common:ndk_helper").projectDir}",
                    "-DANDROID_STL=c++_static",
                    "-DANDROID_TOOLCHAIN=clang"
                )
            }
        }
    }

    buildTypes {
        debug {
            applicationIdSuffix = ".dev"
            isDebuggable = true
            isMinifyEnabled = false
            resValue(type = "string", name = "app_name", value = appNameDev)
        }
        release {
            isDebuggable = false
            isMinifyEnabled = true
            isShrinkResources = true
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
            signingConfig = signingConfigs.getByName("debug")
            resValue(type = "string", name = "app_name", value = appNameProd)
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
        //compose = true
    }
    externalNativeBuild {
        cmake {
            ndkVersion = "26.1.10909125"
            path = file("CMakeLists.txt")
            version = "3.22.1"
        }
    }
}

dependencies {
    // project
    implementation(project(":common:ndk_helper"))

    // libs
    api(libs.game.activity)
    implementation(libs.core.ktx)
    implementation(libs.androidx.appcompat)
    implementation(libs.material)
    implementation(libs.constraintlayout)
    //compose
    //implementation(platform(libs.compose.bom))
    //implementation(libs.material3)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.test.ext)
    androidTestImplementation(libs.androidx.test.espresso)
    androidTestImplementation(platform(libs.compose.bom))
}