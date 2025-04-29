package com.temple.skiaui.compose.example.lyric

import com.temple.skiaui.HYSkiaUIApp

class Lyric(
    val content: String = "",
    val timeMills: LongArray = longArrayOf(),
    val contentList: Array<String> = arrayOf(),
    val lineIndex: Int = 0
) {
    override fun toString(): String {
        return "[Line$lineIndex] ${timeMills.joinToString()} → ${contentList.joinToString()}"
    }
}

fun parseAssetsLrc(path: String): List<Lyric> {
    val lrcText = HYSkiaUIApp.getInstance().assets.open(path).use { inputStream ->
        inputStream.bufferedReader(Charsets.UTF_8).use { reader ->
            reader.readText()
        }
    }
    return parseEnhancedLrc(lrcText)
}

private fun parseEnhancedLrc(lrcText: String): ArrayList<Lyric> {
    val lyricList = ArrayList<Lyric>()
    val timeRegex = Regex("""\d{2}:\d{2}:\d{2},\d{3}""")
    lrcText.lines().filter { it.isNotBlank() }.forEachIndexed { index, line ->
        val timeMatches = timeRegex.findAll(line).map { it.value }.toList()
        val cleanText = line.replace(timeRegex, "")
            .replace(" ", "")
            .replace("-", "") // 处理连字符分隔的时间

        val timeMills = timeMatches.map { timeStr ->
            val (h, m, sMs) = timeStr.split(":")
            val (s, ms) = sMs.split(",")
            h.toLong() * 3600000 + m.toLong() * 60000 + s.toLong() * 1000 + ms.toLong()
        }.toLongArray()
        if (index % 3 == 2) {
            lyricList.add(
                Lyric(
                    content = cleanText,
                    timeMills = timeMills,
                    contentList = cleanText.split("").filter { it.isNotBlank() }.toTypedArray(),
                    lineIndex = lyricList.size
                )
            )
        }
    }
    return lyricList
}
