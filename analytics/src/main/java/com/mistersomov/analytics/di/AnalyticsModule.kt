package com.mistersomov.analytics.di

import com.mistersomov.analytics.data.repository.AnalyticsRepositoryImpl
import com.mistersomov.analytics.domain.FirstLaunchPrefsImpl
import com.mistersomov.analytics_api.domain.FirstLaunchPrefs
import com.mistersomov.analytics_api.domain.repository.AnalyticsRepository
import org.koin.android.ext.koin.androidContext
import org.koin.dsl.module

val analyticsModule = module {
    single<AnalyticsRepository> { AnalyticsRepositoryImpl() }
    single<FirstLaunchPrefs> { FirstLaunchPrefsImpl(androidContext()) }
}