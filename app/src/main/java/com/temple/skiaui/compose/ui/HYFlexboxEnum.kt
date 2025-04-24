package com.temple.skiaui.compose.ui

enum class FlexWrap(val value: String) {
    Unspecified("Unspecified"),
    NoWrap("nowrap"),
    Wrap("wrap"),
    WrapReverse("wrap-reverse")
}

enum class FlexDirection(val value: String) {
    Unspecified("Unspecified"),
    Row("row"),
    RowReverse("row-reverse"),
    Column("column"),
    ColumnReverse("column-reverse")
}

enum class Justify(val value: String) {
    Unspecified("Unspecified"),
    FlexStart("flex-start"),
    FlexEnd("flex-end"),
    Center("center"),
    SpaceBetween("space-between"),
    SpaceAround("space-around"),
    SpaceEvenly("space-evenly")
}

enum class Align(val value: String) {
    Unspecified("Unspecified"),
    FlexStart("flex-start"),
    Center("center"),
    FlexEnd("flex-end"),
    Stretch("stretch"),
    BaseLine("baseline"),
    SpaceBetween("space-between"),
    SpaceAround("space-around"),
    Auto("auto")
}

enum class ScaleType(val value: String) {
    Unspecified("Unspecified"),
    FitXY("fill"),
    CenterCrop("cover"),
    FitCenter("container")
}

enum class Position(val value: String) {
    Unspecified("Unspecified"),
    Absolute("absolute"),
    Static("static"),
    Relative("relative")
}

enum class TextAlign(val value: String) {
    Unspecified("Unspecified"),
    kLeft("left"),
    kRight("right"),
    kCenter("center"),
    kJustify("justify"),
    kStart("start"),
    kEnd("end"),
}

enum class ContentScale(val value: String) {
    Unspecified("Unspecified"),
    Fill("fill"),
    Cover("cover"),
    Contain("contain")
}

