package com.temple.skiaui.keyboard

import android.content.Context.INPUT_METHOD_SERVICE
import android.os.Handler
import android.os.Looper
import android.text.Editable
import android.text.TextWatcher
import android.view.View
import android.view.ViewGroup
import android.view.inputmethod.InputMethodManager
import android.widget.EditText
import androidx.appcompat.app.AppCompatActivity
import com.temple.skiaui.HYSkiaEngine
import com.temple.skiaui.HYSkiaUIApp


class HYKeyboard(val engine: HYSkiaEngine, val inputViewPtr: Long) {

    private val mainHandler = Handler(Looper.getMainLooper())
    private var content = ""

    private val imm =
        HYSkiaUIApp.getInstance().getSystemService(INPUT_METHOD_SERVICE) as InputMethodManager

    fun show(content: String) {
        mainHandler.post {
            getEditTextView().apply {
                requestFocus()
                setText(content)
                setSelection(content.length)
            }
            mainHandler.postDelayed({
                imm.toggleSoftInput(
                    InputMethodManager.SHOW_FORCED,
                    InputMethodManager.HIDE_IMPLICIT_ONLY
                )
                update(inputViewPtr)
            }, 100)
        }
    }

    fun hide() {
        mainHandler.post {
            val view = getEditTextView()
            imm.hideSoftInputFromWindow(view.windowToken, 0)
            view.clearFocus()
        }
    }

    fun release() {
        val window = (engine.view.context as AppCompatActivity).window
        val editText = window.decorView.findViewWithTag<View>("keyboardTagView$inputViewPtr");
        (window.decorView as ViewGroup).removeView(editText)
    }

    fun getContent(): String {
        return content
    }

    private fun getEditTextView(): EditText {
        val window = (engine.view.context as AppCompatActivity).window
        var view = window.currentFocus
        if (view == null) {
            val decorView: View = window.decorView
            val focusView = decorView.findViewWithTag<View>("keyboardTagView$inputViewPtr")
            if (focusView == null) {
                view = EditText(window.context).apply {
                    addTextChangedListener(object : TextWatcher {
                        override fun beforeTextChanged(
                            s: CharSequence?,
                            start: Int,
                            count: Int,
                            after: Int
                        ) {

                        }

                        override fun onTextChanged(
                            s: CharSequence?,
                            start: Int,
                            before: Int,
                            count: Int
                        ) {

                        }

                        override fun afterTextChanged(s: Editable?) {
                            engine.postToSkiaUI {
                                content = s?.toString() ?: ""
                            }
                        }

                    })
                }
                view.setTag("keyboardTagView")
                (decorView as ViewGroup).addView(view, 0, 0)
            } else {
                view = focusView
            }
            view.requestFocus()
        }
        return view as EditText
    }

    private fun update(newFocusedView: Long) {
        if (focusedView != 0L) {
            engine.postToSkiaUI {
                engine.setFocus(focusedView, false)
            }
        }
        if (newFocusedView != 0L) {
            focusedView = newFocusedView
            engine.postToSkiaUI {
                engine.setFocus(focusedView, true)
            }
        }
    }

    companion object {
        private const val TAG = "HYKeyboard"
        private var focusedView = 0L
    }

}