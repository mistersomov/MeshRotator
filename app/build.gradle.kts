import com.google.firebase.crashlytics.buildtools.gradle.CrashlyticsExtension

plugins {
    id("com.mistersomov.app")
    id("com.mistersomov.app.compose")
    alias(libs.plugins.ksp)
    alias(libs.plugins.googleServices)
    alias(libs.plugins.firebaseCrashlytics)
}

val appNameDev = "MeshRotator Dev"
val appNameProd = "MeshRotator"

android {
    namespace = "com.mistersomov.meshrotator"

    defaultConfig {
        applicationId = "com.mistersomov.meshrotator"
        versionCode = libs.versions.versionCode.get().toInt()
        versionName = libs.versions.versionName.get()

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        externalNativeBuild {
            cmake {
                cppFlags += "-std=c++11 -frtti -Wall -Werror"
                arguments(
                    "-DNDK_HELPER_PATH=${project(":common:ndk_helper").projectDir}",
                    "-DNATIVE_TESTING_PATH=${project(":native-testing").projectDir}",
                    "-DANDROID_STL=c++_static",
                    "-DANDROID_TOOLCHAIN=clang"
                )
            }
        }
    }
    externalNativeBuild {
        cmake {
            ndkVersion = "26.1.10909125"
            path = file("CMakeLists.txt")
            version = "3.22.1"
        }
    }
    buildTypes {
        getByName("debug") {
            applicationIdSuffix = ".dev"
            isDebuggable = true
            isMinifyEnabled = false
            resValue(type = "string", name = "app_name", value = appNameDev)
        }
        getByName("release") {
            isDebuggable = false
            isMinifyEnabled = true
            isShrinkResources = true
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
            signingConfig = signingConfigs.getByName("debug")
            resValue(type = "string", name = "app_name", value = appNameProd)
            configure<CrashlyticsExtension> {
                nativeSymbolUploadEnabled = true
            }
        }
    }
    packaging {
        resources {
            excludes += "/META-INF/{AL2.0,LGPL2.1}"
        }
    }
}

dependencies {
    // project
    implementation(project(":common:ndk_helper"))
    implementation(project(":native-testing"))

    // libs
    api(libs.game.activity)
    implementation(libs.core.ktx)
    implementation(libs.androidx.appcompat)
    implementation(libs.material)
    implementation(libs.constraintlayout)

    // analytics
    api(platform(libs.firebase.bom))
    implementation(libs.firebaseCrashlytics)
    implementation(libs.firebaseCrashlyticsNdk)

    // tests
    implementation(libs.androidx.junit.gtest)
    implementation(libs.googletest)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.test.ext)
    androidTestImplementation(libs.androidx.test.espresso)
    androidTestImplementation(platform(libs.compose.bom))
}