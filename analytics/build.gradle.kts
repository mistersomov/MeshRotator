plugins {
    id("com.mistersomov.core")
}

android {
    namespace = "com.mistersomov.analytics"
}

dependencies {
    api(project(":analytics-api"))

    api(platform(libs.firebase.bom))
    api(libs.firebase.analytics.ktx)
}