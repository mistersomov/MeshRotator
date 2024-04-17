package com.mistersomov.meshrotator

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup.LayoutParams
import android.view.ViewGroup.LayoutParams.MATCH_PARENT
import com.google.androidgamesdk.GameActivity
import com.mistersomov.analytics_api.data.model.Event.FIRST_LAUNCH
import com.mistersomov.analytics_api.domain.FirstLaunchPrefs
import com.mistersomov.analytics_api.domain.repository.AnalyticsRepository
import com.mistersomov.design.component.joystick.VirtualJoystick
import com.mistersomov.meshrotator.databinding.MainSceneLayoutBinding
import org.koin.android.ext.android.get

class MainActivity : GameActivity() {
    companion object {
        init {
            System.loadLibrary("meshrotator")
        }
    }

    private lateinit var binding: MainSceneLayoutBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val firstLaunchPrefs: FirstLaunchPrefs = get()
        val analyticsRepository: AnalyticsRepository = get()
        if (firstLaunchPrefs.isFirstLaunch()) {
            firstLaunchPrefs.onFirstLaunch()
            analyticsRepository.sendEventFB(FIRST_LAUNCH)
        }
        binding = MainSceneLayoutBinding.inflate(LayoutInflater.from(this))
        attachJoystick()
    }

    override fun onWindowFocusChanged(hasFocus: Boolean) {
        super.onWindowFocusChanged(hasFocus)
        if (hasFocus) {
            hideSystemUi()
        }
    }

    private fun hideSystemUi() {
        window.decorView.systemUiVisibility = (View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                or View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                or View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                or View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                or View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                or View.SYSTEM_UI_FLAG_FULLSCREEN)
    }

    private fun attachJoystick() {
        binding.vJoystick.apply {
            setContent {
                VirtualJoystick { _, _ ->

                }
            }
        }
        addContentView(binding.root, LayoutParams(MATCH_PARENT, MATCH_PARENT))
    }
}