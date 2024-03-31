import com.android.build.api.dsl.ApplicationExtension
import com.mistersomov.convention.configureKotlinAndroid
import com.mistersomov.convention.utils.libs
import org.gradle.api.Plugin
import org.gradle.api.Project
import org.gradle.kotlin.dsl.configure

class AndroidAppConventionPlugin : Plugin<Project> {
    override fun apply(target: Project) {
        with(target) {
            with(pluginManager) {
                apply("com.android.application")
                apply("org.jetbrains.kotlin.android")
            }
            extensions.configure<ApplicationExtension> {
                configureKotlinAndroid(this)
                defaultConfig {
                    targetSdk = libs.findVersion("targetSdk").get().toString().toInt()
                }
                buildFeatures {
                    prefab = true
                    viewBinding = true
                }
            }
        }
    }
}