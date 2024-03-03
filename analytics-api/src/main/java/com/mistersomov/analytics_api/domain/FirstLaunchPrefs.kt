package com.mistersomov.analytics_api.domain

interface FirstLaunchPrefs {
    fun isFirstLaunch(): Boolean
    fun onFirstLaunch()
    fun isFirstLaunchAfterUpdate(): Boolean
    fun onFirstLaunchAfterUpdate()
}