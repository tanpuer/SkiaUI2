#pragma once

#include <iostream>
#include <sstream>
#include <string>

static void hexToRGBA(const std::string& hex, int& r, int& g, int& b, int& a) {
    // 检查颜色格式是否正确（#rgb 或 #rgba）
    if (hex.size() != 7 && hex.size() != 9) {
        return;
    }
    if (hex[0] != '#') {
        return;
    }

    // 使用字符串流读取颜色值
    std::stringstream ss;

    // 设置默认的Alpha值为最大值 255 (不透明)
    a = 255;

    // 转换red部分
    ss << std::hex << hex.substr(1, 2);
    ss >> r;
    ss.clear(); // 清除stringstream的状态

    // 转换green部分
    ss << std::hex << hex.substr(3, 2);
    ss >> g;
    ss.clear();

    // 转换blue部分
    ss << std::hex << hex.substr(5, 2);
    ss >> b;
    ss.clear();

    // 如果存在Alpha值则转换Alpha部分
    if (hex.size() == 9) {
        ss << std::hex << hex.substr(7, 2);
        ss >> a;
        ss.clear();
    }
}

static std::string SkColorToString(SkColor color) {
    // 分别获取alpha、红色、绿色和蓝色分量的值
    int alpha = SkColorGetA(color);
    int red = SkColorGetR(color);
    int green = SkColorGetG(color);
    int blue = SkColorGetB(color);

    // 使用流操作符创建十六进制表示的字符串
    std::stringstream ss;
    ss << "#" << std::hex << std::setw(2) << std::setfill('0') << alpha
       << std::hex << std::setw(2) << std::setfill('0') << red
       << std::hex << std::setw(2) << std::setfill('0') << green
       << std::hex << std::setw(2) << std::setfill('0') << blue;

    return ss.str();
}
