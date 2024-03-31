plugins {
    id("com.mistersomov.library")
    id("com.mistersomov.library.compose")
}

android {
    namespace = "com.mistersomov.design"
}

dependencies {
    implementation(libs.core.ktx)
    implementation(libs.androidx.appcompat)
    implementation(libs.material)

    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.test.ext)
    androidTestImplementation(libs.androidx.test.espresso)
}