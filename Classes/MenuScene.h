#pragma once

#include "cocos2d.h"

class MenuScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

    CREATE_FUNC(MenuScene);

private:
    cocos2d::Node* offsetNode = nullptr;                              // 偏移
    cocos2d::Sprite* background = nullptr;                            // 背景图片
    cocos2d::Sprite* selector = nullptr;                              // 菜单选择器
    bool _isSelectorInit = false;

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,        // 键盘移动光标
                      cocos2d::Event* event);
    bool onTouch(cocos2d::Touch* touch, cocos2d::Event*);             // 触摸移动光标

    void _initBackground();
    void _initSelector();

    cocos2d::Animate* _getAnimFrames();                              // 选择器帧动画
};
