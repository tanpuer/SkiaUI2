package com.temple.skiaui.compose.ui

enum class FlexWrap(val value: String) {
    NoWrap("nowrap"),
    Wrap("wrap"),
    WrapReverse("wrap-reverse")
}

enum class FlexDirection(val value: String) {
    Row("row"),
    RowReverse("row-reverse"),
    Column("column"),
    ColumnReverse("column-reverse")
}

enum class Justify(val value: String) {
    FlexStart("flex-start"),
    FlexEnd("flex-end"),
    Center("center"),
    SpaceBetween("space-between"),
    SpaceAround("space-around"),
    SpaceEvenly("space-evenly")
}

enum class Align(val value: String) {
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
    FitXY("fill"),
    CenterCrop("cover"),
    FitCenter("container")
}

enum class Position(val value: String) {
    Absolute("absolute"),
    Static("static"),
    Relative("relative")
}