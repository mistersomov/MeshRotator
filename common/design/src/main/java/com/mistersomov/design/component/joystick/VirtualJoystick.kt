package com.mistersomov.design.component.joystick

import android.view.MotionEvent.ACTION_DOWN
import android.view.MotionEvent.ACTION_MOVE
import androidx.compose.animation.core.Animatable
import androidx.compose.foundation.Canvas
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.BoxWithConstraints
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.runtime.Composable
import androidx.compose.runtime.MutableFloatState
import androidx.compose.runtime.mutableFloatStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.ui.Alignment
import androidx.compose.ui.ExperimentalComposeUiApi
import androidx.compose.ui.Modifier
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.drawscope.Stroke
import androidx.compose.ui.input.pointer.pointerInteropFilter
import androidx.compose.ui.tooling.preview.Preview
import kotlinx.coroutines.launch
import kotlin.math.pow
import kotlin.math.sqrt

private const val ALPHA_DEFAULT = 0.2f
private const val ALPHA_PRESSED = 0.5f
private const val OUTER_STROKE_WIDTH = 5.0f

@OptIn(ExperimentalComposeUiApi::class)
@Composable
fun VirtualJoystick(
    modifier: Modifier = Modifier,
    onJoystickMoved: (xPercent: Float, yPercent: Float) -> Unit
) {
    BoxWithConstraints(
        modifier = Modifier
            .fillMaxSize()
            .then(modifier),
        contentAlignment = Alignment.Center,
    ) {
        val scope = rememberCoroutineScope()
        val centerX: Float = constraints.maxWidth.toFloat() / 2
        val centerY: Float = constraints.maxHeight.toFloat() / 2
        val baseRadius: Float = remember {
            constraints.maxWidth.coerceAtMost(constraints.maxHeight).toFloat() / 3
        }
        val knobRadius: Float = remember {
            constraints.maxWidth.coerceAtMost(constraints.maxHeight).toFloat() / 6
        }
        val knobX: MutableFloatState = remember { mutableFloatStateOf(centerX) }
        val knobY: MutableFloatState = remember { mutableFloatStateOf(centerY) }
        val joystickAlpha = remember { Animatable(ALPHA_DEFAULT) }

        Box(
            modifier = Modifier
                .fillMaxSize()
                .align(Alignment.Center)
                .pointerInteropFilter { event ->
                    when (event.action) {
                        ACTION_DOWN, ACTION_MOVE -> {
                            val distance =
                                sqrt((event.x - centerX).pow(2) + (event.y - centerY).pow(2))

                            scope.launch { joystickAlpha.animateTo(ALPHA_PRESSED) }
                            if (distance < baseRadius) {
                                knobX.floatValue = event.x
                                knobY.floatValue = event.y
                                val xPercent = (knobX.floatValue - centerX) / baseRadius
                                val yPercent = (knobY.floatValue - centerY) / baseRadius
                                onJoystickMoved(xPercent, yPercent)
                            } else {
                                val ratio = baseRadius / distance
                                val constrainedX = centerX + (event.x - centerX) * ratio
                                val constrainedY = centerY + (event.y - centerY) * ratio
                                knobX.floatValue = constrainedX
                                knobY.floatValue = constrainedY
                                val xPercent = (knobX.floatValue - centerX) / baseRadius
                                val yPercent = (knobY.floatValue - centerY) / baseRadius
                                onJoystickMoved(xPercent, yPercent)
                            }
                        }
                        else -> {
                            scope.launch { joystickAlpha.animateTo(ALPHA_DEFAULT) }
                            knobX.floatValue = centerX
                            knobY.floatValue = centerY
                            onJoystickMoved(0.0f, 0.0f)
                        }
                    }
                    true
                },
            contentAlignment = Alignment.Center
        ) {
            Canvas(
                modifier = Modifier
                    .fillMaxSize()
                    .background(Color.Transparent)
            ) {
                drawCircle(
                    color = Color.DarkGray,
                    radius = baseRadius,
                    style = Stroke(OUTER_STROKE_WIDTH),
                    alpha = joystickAlpha.value
                )
                drawCircle(
                    color = Color.DarkGray,
                    radius = knobRadius,
                    alpha = joystickAlpha.value,
                    center = Offset(knobX.floatValue, knobY.floatValue)
                )
            }
        }
    }
}

@Preview(
    widthDp = 150,
    heightDp = 150,
)
@Composable
fun PreviewVirtualJoystick() {
    VirtualJoystick { _, _ ->

    }
}