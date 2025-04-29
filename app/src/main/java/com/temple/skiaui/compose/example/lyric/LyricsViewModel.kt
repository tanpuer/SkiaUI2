package com.temple.skiaui.compose.example.lyric

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch

class LyricsViewModel : ViewModel() {

    private val _lyrics = MutableStateFlow<List<Lyric>>(emptyList())

    val lyrics: StateFlow<List<Lyric>> = _lyrics.asStateFlow()

    private val _progress = MutableStateFlow<Long>(0L)

    val progress: StateFlow<Long> = _progress.asStateFlow()

    fun parseLrc(path: String) {
        viewModelScope.launch(Dispatchers.IO) {
            val lyrics = parseAssetsLrc(path)
            viewModelScope.launch(Dispatchers.Main) {
                _lyrics.value = lyrics
            }
        }
    }

    fun setProgress(progress: Long) {
        _progress.value = progress
    }

}