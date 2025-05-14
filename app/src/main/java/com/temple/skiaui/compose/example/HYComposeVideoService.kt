package com.temple.skiaui.compose.example

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.content.Intent
import android.os.IBinder
import com.temple.skiaui.HYSkiaUIApp
import com.temple.skiaui.R

class HYComposeVideoService : Service() {

    private val notificationManager =
        HYSkiaUIApp.getInstance().getSystemService(NOTIFICATION_SERVICE) as NotificationManager

    override fun onBind(intent: Intent?): IBinder? {
        return null
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        val channel =
            NotificationChannel(CHANNEL_ID, CHANNEL_NAME, NotificationManager.IMPORTANCE_DEFAULT)
        notificationManager.createNotificationChannel(channel)
        val notification = Notification.Builder(applicationContext, CHANNEL_ID)
            .setContentText(getString(R.string.music_playing))
            .setContentTitle(getString(R.string.music_playing))
            .setSmallIcon(R.drawable.ic_launcher_foreground)
            .build()
        notificationManager.notify(NOTIFICATION_ID, notification)
        startForeground(NOTIFICATION_ID, notification)
        return START_NOT_STICKY
    }

    override fun onDestroy() {
        super.onDestroy()
        notificationManager.cancel(NOTIFICATION_ID)
    }

    companion object {
        private const val CHANNEL_ID = "compose_video"
        private const val CHANNEL_NAME = "compose_video_notification"
        private const val NOTIFICATION_ID = 1010
    }
}