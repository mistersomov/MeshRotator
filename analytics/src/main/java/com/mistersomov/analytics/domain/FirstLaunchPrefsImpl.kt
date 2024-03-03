package com.mistersomov.analytics.domain

import android.content.Context
import android.content.SharedPreferences
import com.mistersomov.analytics_api.domain.FirstLaunchPrefs

internal class FirstLaunchPrefsImpl(private val context: Context) : FirstLaunchPrefs {
    private companion object {
        const val FIRST_LAUNCH_PREFS_FILE_NAME = "first_launch_prefs"
        const val ARGS_IS_FIRST_LAUNCH = "is_first_launch"
        const val ARGS_IS_FIRST_LAUNCH_AFTER_UPDATE = "is_first_launch_after_update"
    }

    private val sharedPreferences: SharedPreferences =
        context.getSharedPreferences(FIRST_LAUNCH_PREFS_FILE_NAME, Context.MODE_PRIVATE)
    private val editor: SharedPreferences.Editor = sharedPreferences.edit()

    override fun isFirstLaunch(): Boolean =
        sharedPreferences.getBoolean(ARGS_IS_FIRST_LAUNCH, true)

    override fun onFirstLaunch() {
        editor.putBoolean(ARGS_IS_FIRST_LAUNCH, false).apply()
    }

    override fun isFirstLaunchAfterUpdate(): Boolean =
        sharedPreferences.getString(
            ARGS_IS_FIRST_LAUNCH_AFTER_UPDATE,
            ""
        ) != getAppVersion(context)

    override fun onFirstLaunchAfterUpdate() {
        editor.putString(ARGS_IS_FIRST_LAUNCH_AFTER_UPDATE, getAppVersion(context)).apply()
    }

    private fun getAppVersion(context: Context): String {
        return context.packageManager.getPackageInfo(context.packageName, 0).versionName
    }
}