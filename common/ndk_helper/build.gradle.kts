plugins {
    id("com.mistersomov.library")
}

android {
    namespace = "com.mistersomov.ndk_helper"

    defaultConfig {
        externalNativeBuild {
            cmake {
                targets += "ndk_helper"
            }
        }
    }
    externalNativeBuild {
        cmake {
            ndkVersion = "26.1.10909125"
            path("CMakeLists.txt")
            version = "3.22.1"
        }
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