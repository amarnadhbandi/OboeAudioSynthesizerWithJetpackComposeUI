package com.demo.audiosynthesizer

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.viewModels
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxHeight
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.width
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.VolumeMute
import androidx.compose.material.icons.filled.VolumeUp
import androidx.compose.material3.Button
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Slider
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.livedata.observeAsState
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.rotate
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.dp
import com.demo.audiosynthesizer.ui.theme.AudioSynthesizerTheme

class MainActivity : ComponentActivity() {
    private val synthesizerViewModel: AudioSynthesizerViewModel by viewModels()
    private val synthesizer = NativeAudioSynthesizer()
    //private val synthesizer = LoggingWaveTableSynthesizer()


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        //requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE

        lifecycle.addObserver(synthesizer)

        synthesizerViewModel.audioSynthesizer = synthesizer
        setContent {
            AudioSynthesizerTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    AudioSynthesizerApp(
                        modifier = Modifier,
                        synthesizerViewModel = synthesizerViewModel
                    )
                }
            }
        }
    }

    override fun onResume() {
        super.onResume()
        synthesizerViewModel.applyParameters()
    }

    override fun onDestroy() {
        super.onDestroy()
        lifecycle.removeObserver(synthesizer)
    }
}


@Composable
fun AudioSynthesizerApp(modifier: Modifier, synthesizerViewModel: AudioSynthesizerViewModel) {
    Column(
        modifier = modifier.fillMaxSize(),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Top
    ) {
        SelectionPanel(modifier, synthesizerViewModel = synthesizerViewModel)
        ControlPanel(modifier = modifier, synthesizerViewModel = synthesizerViewModel)
    }
}

@Composable
fun SelectionPanel(modifier: Modifier, synthesizerViewModel: AudioSynthesizerViewModel) {
    Row(
        modifier = modifier
            .fillMaxWidth()
            .fillMaxHeight(0.5f),
        horizontalArrangement = Arrangement.SpaceAround,
        verticalAlignment = Alignment.CenterVertically
    )
    {
        Column(
            modifier = modifier
                .fillMaxSize(),
            verticalArrangement = Arrangement.SpaceEvenly,
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Text(stringResource(id = R.string.audio_table))
            SelectionTableButtons(modifier = modifier, synthesizerViewModel = synthesizerViewModel)
        }
    }
}


@Composable
fun SelectionTableButtons(modifier: Modifier, synthesizerViewModel: AudioSynthesizerViewModel) {
    Row(
        modifier = modifier.fillMaxWidth(),
        horizontalArrangement = Arrangement.SpaceEvenly,
        verticalAlignment = Alignment.Top
    ) {
        for (audiotable in Wavetable.values()) {
            AudiotableButton(
                modifier = modifier,
                onClick = { synthesizerViewModel.setWaveTable(newWavetable = audiotable) },
                label = stringResource(audiotable.toResourceString())
            )
        }
    }
}

@Composable
fun AudiotableButton(
    modifier: Modifier,
    onClick: () -> Unit,
    label: String,
) {
    Button(modifier = modifier, onClick = onClick) {
        Text(text = label)
    }
}


@Composable
fun ControlPanel(modifier: Modifier, synthesizerViewModel: AudioSynthesizerViewModel) {
    Row(
        modifier = modifier
            .fillMaxWidth()
            .fillMaxHeight(),
        horizontalArrangement = Arrangement.Center,
        verticalAlignment = Alignment.CenterVertically
    ) {
        Column(
            modifier = modifier
                .fillMaxHeight()
                .fillMaxWidth(0.7f),
            horizontalAlignment = Alignment.CenterHorizontally,
            verticalArrangement = Arrangement.Center
        ) {
            PitchControl(modifier, synthesizerViewModel = synthesizerViewModel)
            PlayControl(modifier, synthesizerViewModel = synthesizerViewModel)
        }

        Column(
            modifier = modifier
                .fillMaxSize()
                .fillMaxWidth(0.3f),
            horizontalAlignment = Alignment.CenterHorizontally,
            verticalArrangement = Arrangement.SpaceEvenly
        ) {
            VolumeControl(modifier = modifier, synthesizerViewModel = synthesizerViewModel)
        }
    }
}

@Composable
fun PitchControl(modifier: Modifier, synthesizerViewModel: AudioSynthesizerViewModel) {
    val frequency = synthesizerViewModel.frequency.observeAsState()
    PitchControlContent(
        modifier = modifier,
        pitchControlLabel = stringResource(R.string.frequency),
        value = synthesizerViewModel.sliderPositionFromFrequencyInHz(frequency.value!!),
        onValueChange = { synthesizerViewModel.setFrequencySliderPosition(it) },
        valueRange = 0f..1f,
        frequencyValueLabel = stringResource(R.string.frequency_value, frequency.value!!)
    )
}

@Composable
fun PitchControlContent(
    modifier: Modifier,
    pitchControlLabel: String,
    value: Float,
    onValueChange: (Float) -> Unit,
    valueRange: ClosedFloatingPointRange<Float>,
    frequencyValueLabel: String,
) {
    Text(pitchControlLabel)
    Slider(
        modifier = modifier,
        value = value,
        onValueChange = onValueChange,
        valueRange = valueRange
    )
    Text(frequencyValueLabel)
}

@Composable
fun PlayControl(modifier: Modifier, synthesizerViewModel: AudioSynthesizerViewModel) {
    val buttonLabel = synthesizerViewModel.playButtonLabel.observeAsState()
    Button(modifier = modifier,
        onClick = { synthesizerViewModel.playClicked() }) {
        Text(stringResource(buttonLabel.value!!))
    }
}

@Composable
fun VolumeControl(
    modifier: Modifier,
    synthesizerViewModel: AudioSynthesizerViewModel,
) {
    val volume = synthesizerViewModel.volume.observeAsState()
    VolumeControlContent(
        modifier = modifier,
        volume = volume.value!!,
        onValueChange = {
            synthesizerViewModel.setVolume(it)
        },
        volumeRange = synthesizerViewModel.volumeRange
    )
}


@Composable
fun VolumeControlContent(
    modifier: Modifier,
    volume: Float,
    onValueChange: (Float) -> Unit,
    volumeRange: ClosedFloatingPointRange<Float>,
) {
    val screenHeight = LocalConfiguration.current.screenHeightDp
    val sliderHeight = screenHeight / 4

    Icon(imageVector = Icons.Filled.VolumeUp, contentDescription = null)
    Slider(
        value = volume,
        onValueChange = onValueChange,
        modifier = modifier
            .width(sliderHeight.dp)
            .rotate(270f),
        valueRange = volumeRange
    )
    Icon(imageVector = Icons.Filled.VolumeMute, contentDescription = null)
}


