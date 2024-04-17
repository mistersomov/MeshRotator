package com.mistersomov.analytics_api.domain.repository

import com.mistersomov.analytics_api.data.model.IEvent
import com.mistersomov.analytics_api.data.model.IKey
import com.mistersomov.analytics_api.data.model.IValue

interface AnalyticsRepository {
    fun sendEventFB(event: IEvent, key: IKey? = null, value: IValue? = null)
}