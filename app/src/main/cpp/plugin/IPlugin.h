//
// Created by ByteDance on 2023/1/6.
//

#ifndef SKIAUI_IPLUGIN_H
#define SKIAUI_IPLUGIN_H

#include "string"

class IPlugin {

public:

    IPlugin() = delete;

    IPlugin(std::string name);

};


#endif //SKIAUI_IPLUGIN_H
