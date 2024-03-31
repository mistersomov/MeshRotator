import com.android.build.api.dsl.ApplicationExtension
import com.mistersomov.convention.configureAndroidCompose
import org.gradle.api.Plugin
import org.gradle.api.Project
import org.gradle.kotlin.dsl.getByType

class AndroidAppComposeConventionPlugin : Plugin<Project> {
    override fun apply(target: Project) {
        with(target) {
            with(pluginManager) {
                apply("com.android.application")
                configureAndroidCompose(extensions.getByType<ApplicationExtension>())
            }
        }
    }
}