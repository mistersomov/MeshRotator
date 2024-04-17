package com.mistersomov.meshrotator

import android.app.Application
import com.mistersomov.analytics.di.analyticsModule
import org.koin.android.ext.koin.androidContext
import org.koin.android.ext.koin.androidLogger
import org.koin.core.context.startKoin
import timber.log.Timber

class MeshRotatorApp : Application() {
    override fun onCreate() {
        super.onCreate()
        initTimber()
        startKoin {
            androidLogger()
            androidContext(this@MeshRotatorApp)
            modules(analyticsModule)
        }
    }

    private fun initTimber() {
        Timber.plant(Timber.DebugTree())
    }
}