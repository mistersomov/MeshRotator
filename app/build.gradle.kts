import com.google.firebase.crashlytics.buildtools.gradle.CrashlyticsExtension
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Properties

plugins {
    id("com.mistersomov.app")
    id("com.mistersomov.app.compose")
    alias(libs.plugins.ksp)
    alias(libs.plugins.googleServices)
    alias(libs.plugins.firebaseCrashlytics)
}

val appNameDev = "MeshRotator Dev"
val appNameProd = "MeshRotator"

val keystorePropertiesFile = rootProject.file("keystore.properties")
val keystoreProperties = Properties()
keystoreProperties.load(keystorePropertiesFile.inputStream())

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
    signingConfigs {
        create("release") {
            keyAlias = keystoreProperties["keyAlias"] as String
            keyPassword = keystoreProperties["keyPassword"] as String
            storeFile = file(keystoreProperties["storeFile"] as String)
            storePassword = keystoreProperties["storePassword"] as String
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
            signingConfig = signingConfigs.getByName("release")
            resValue(type = "string", name = "app_name", value = appNameProd)
            configure<CrashlyticsExtension> {
                nativeSymbolUploadEnabled = true
            }
            applicationVariants.configureEach {
                val date = Date()
                val formattedDate = SimpleDateFormat("yyyy-MM-dd").format(date)
                val apkName = "${appNameProd}-${defaultConfig.versionName}-${formattedDate}.apk"

                outputs.configureEach {
                    buildOutputs.all {
                        val output =
                            this as com.android.build.gradle.internal.api.BaseVariantOutputImpl
                        output.outputFileName = apkName
                    }
                }
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
    implementation(project(":common:design"))

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