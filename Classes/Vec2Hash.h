#include "cocos/math/Vec2.h"

namespace std {

template<>
class hash<cocos2d::Vec2>
{
public:
    // Ref: https://blog.csdn.net/hellokandy/article/details/126599351
    std::size_t operator() (const cocos2d::Vec2& v) const
    {
        std::size_t h1 = std::hash<int>{}((int)v.x);
        std::size_t h2 = std::hash<int>{}((int)v.y);
        return h1 ^ (h2 << 1);
    }
};

}
