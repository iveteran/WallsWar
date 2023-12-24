#pragma once

enum class NetworkingMode {
    NONE,
    SINGLE,
    LAN,
    ONLINE,
};

const char* getNetworkingModeName(NetworkingMode nm);
