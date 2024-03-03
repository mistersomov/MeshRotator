plugins {
    `kotlin-dsl`
}

group = "com.mistersomov.build_logic"

java {
    sourceCompatibility = JavaVersion.VERSION_17
    targetCompatibility = JavaVersion.VERSION_17
}

dependencies {
    compileOnly(libs.gradle)
    compileOnly(libs.kotlin.gradlePlugin)
}

gradlePlugin {
    plugins {
        register("app") {
            id = "com.mistersomov.app"
            implementationClass = "AndroidAppConventionPlugin"
        }
        register("appCompose") {
            id = "com.mistersomov.app.compose"
            implementationClass = "AndroidAppComposeConventionPlugin"
        }
        register("library") {
            id = "com.mistersomov.library"
            implementationClass = "AndroidLibraryConventionPlugin"
        }
        register("libraryCompose") {
            id = "com.mistersomov.library.compose"
            implementationClass = "AndroidLibraryComposeConventionPlugin"
        }
        register("api") {
            id = "com.mistersomov.api"
            implementationClass = "AndroidApiConventionPlugin"
        }
        register("core") {
            id = "com.mistersomov.core"
            implementationClass = "AndroidCoreConventionPlugin"
        }
    }
}