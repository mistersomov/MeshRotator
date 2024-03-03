import com.android.build.api.dsl.LibraryExtension
import com.mistersomov.convention.configureKotlinAndroid
import com.mistersomov.convention.utils.libs
import org.gradle.api.Plugin
import org.gradle.api.Project
import org.gradle.kotlin.dsl.configure
import org.gradle.kotlin.dsl.dependencies

class AndroidCoreConventionPlugin : Plugin<Project> {
    override fun apply(target: Project) {
        with(target) {
            with(pluginManager) {
                apply("com.mistersomov.library")
            }
            extensions.configure<LibraryExtension> {
                testOptions {
                    unitTests {
                        all {
                            it.useJUnitPlatform()
                        }
                    }
                }
            }
            dependencies {
                add("implementation", libs.findLibrary("kotlinx-coroutines-core").get())
                add("implementation", libs.findLibrary("kotlinx-coroutines-android").get())
                add("implementation", libs.findLibrary("timber").get())
                add("implementation", libs.findLibrary("koin").get())
                add("testImplementation", libs.findLibrary("junit5-engine").get())
                add("testImplementation", libs.findLibrary("junit5").get())
                add("testImplementation", libs.findLibrary("test-mockk").get())
                add("testImplementation", libs.findLibrary("koin-test").get())
            }
        }
    }
}