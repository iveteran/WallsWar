#pragma once

#include "cocos2d.h"
#include "Bullet.h"
#include "Common.h"

// 坦克等级
// 玩家分为4级
// 2级及以上可发射两枚子弹
// 敌人分为4种
using TankLevel = short;

class TankBase : public cocos2d::Sprite {
public:
    bool init() override;                               // 初始化缓存和子弹

    virtual void playAnimate();                         // 播放移动动画
    virtual void stopAnimate();                         // 停止播放动画
    virtual void shoot();                               // 发射子弹
    virtual void setDirection(Direction dir) = 0;       // 坦克改变方向

    void startMove(Direction dir);                      // 开启自动移动
    void stopMove();                                    // 停止自动移动
    void birth(std::string);                            // 坦克出生动画
    void beInvincible(int);                             // 使坦克无敌
    virtual void disBlood();                            // 坦克掉血

    static void addSpriteFrameCache();

    cocos2d::Vector<Bullet*>& getAllBullets();          // 获得坦克拥有的所有子弹
    Bullet* getBullet1();                               // 得到第一颗子弹
    bool canMove = false;                               // 判断能否移动（刚出生时一段时间内无法移动）

protected:
    virtual void _initBullets() = 0;                   // 创建子弹
    virtual const cocos2d::Vector<cocos2d::Animate*>*
        _getAnimations() = 0;                          // 获取帧动画

    void _autoMove(float t);                           // 自动移动
    void _makeCameraFollowPlayerByMapBorder();
    void _adjustPosition();                            // 调整位置为8的倍数
    void _shoot(Bullet* bullet);                       // 发射子弹辅助函数

    Direction _dir = Direction::UP;                      // 坦克当前方向
    TankLevel _level = 0;                                // 坦克当前等级
    cocos2d::Vector<Bullet*> _bullets;                   // 存储坦克所有的子弹
    unsigned char _blood = 1;                            // 坦克生命值
    bool _isInvincible = false;                          // 判断是否无敌
    int _moveDistance = 0;                               // 移动总距离

private:
    static float _adjustNumber(int number);            // 将给定数字调整为8的倍数
    bool _isMapIntersection();                         // 检测坦克与地图边缘的碰撞
    bool _isBlockIntersection();                       // 检测坦克与方块的碰撞
    virtual bool _isTankIntersection() = 0;            // 检测坦克之间的碰撞

    int _musicId = -1;                                   // 移动时播放的音乐id
    bool _isMove = false;                                // 判断是否正在移动
};
