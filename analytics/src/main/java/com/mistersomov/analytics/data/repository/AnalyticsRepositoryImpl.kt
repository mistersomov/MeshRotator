package com.mistersomov.analytics.data.repository

import androidx.core.os.bundleOf
import com.google.firebase.analytics.FirebaseAnalytics
import com.google.firebase.analytics.ktx.analytics
import com.google.firebase.ktx.Firebase
import com.mistersomov.analytics_api.data.model.IEvent
import com.mistersomov.analytics_api.data.model.IKey
import com.mistersomov.analytics_api.data.model.IValue
import com.mistersomov.analytics_api.domain.repository.AnalyticsRepository
import timber.log.Timber

internal class AnalyticsRepositoryImpl : AnalyticsRepository {
    private val firebase: FirebaseAnalytics = Firebase.analytics

    override fun sendEventFB(event: IEvent, key: IKey?, value: IValue?) {
        val params = bundleOf("key" to key?.title, "value" to value?.title)

        firebase.logEvent(event.title, params)
        Timber.d("[Firebase] Sent event=${event.title}, params=$params")
    }

}