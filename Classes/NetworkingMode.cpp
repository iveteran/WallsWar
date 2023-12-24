#include "NetworkingMode.h"

const char* getNetworkingModeName(NetworkingMode nm) {
    const char* nmName = nullptr;
    switch (nm) {
        case NetworkingMode::SINGLE:
            nmName = "单机模式";
            break;
        case NetworkingMode::LAN:
            nmName = "本地网络模式";
            break;
        case NetworkingMode::ONLINE:
            nmName = "线上网络模式";
            break;
        default:
            break;
    }
    return nmName;
}
