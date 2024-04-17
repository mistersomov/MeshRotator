package com.mistersomov.analytics_api.data.model

interface IEvent {
    val title: String
}

interface IKey {
    val title: String
}

interface IValue {
    val title: String
}

enum class Event(override val title: String) : IEvent {
    FIRST_LAUNCH("first_launch"),
    TEST("test"),
}

enum class Key(override val title: String) : IKey {
    SOURCE("Source"),
    STATUS("Status"),
    ACTIONS("Actions"),
}

enum class Value(override val title: String) : IValue {
    SUCCESS("Success"),
    FAILURE("Failure"),
    LONG_TAP("Long tap"),
}