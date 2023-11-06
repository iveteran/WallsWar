#ifndef _POPUP_LAYER_H_
#define _POPUP_LAYER_H_

#include "cocos2d.h"
#include "ui/UILayout.h"

USING_NS_CC;

namespace cocos2d::ui {
    class Text;
}
using namespace cocos2d::ui;
 
class PopupLayer : public Layout {
public:
    CREATE_FUNC(PopupLayer);

    bool init(int width, int height, const Color3B& color, int opacity, const char* bgImage=nullptr);
     
    void setFont(const char* font, int size, float scale);
    void setTitle(const char* title);
    void setTitleBackgroundColor(const Color3B& color);
    int getTitleHeight() const;
    float getWidth() const;
    float getHeight() const;

    void addMainPanel(Widget* widget);

protected:
    virtual bool init() { return true; }
    void showEffect();
     
    // 需要重写触摸事件处理函数，达到“模态”效果
    virtual void setSwallowTouches();

    // 重写触摸函数，返回true，屏蔽祖先层，达到“模态”效果
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
 
private:
    Layout* _layout = nullptr;
    Text* _titleLabel = nullptr;

    Color3B _titleBgColor = Color3B(41, 74, 122);  // 深蓝色
    const char* _font = nullptr;
    int _fontSize = 12;
    float _fontScale = 0.5f;
};

#endif  // _POPUP_LAYER_H_
