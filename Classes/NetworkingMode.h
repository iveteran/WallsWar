#pragma once

enum class NetworkingMode {
    NONE,
    SINGLE,
    LOCAL,
    ONLINE,
};

const char* getNetworkingModeName(NetworkingMode nm);
