#ifndef _POPUP_LAYER_H_
#define _POPUP_LAYER_H_

#include "cocos2d.h"
#include "ui/UILayout.h"

USING_NS_CC;

namespace cocos2d::ui {
    class Text;
}
using namespace cocos2d::ui;

class PopupLayer;

using dialogClosedCallback = std::function<void(PopupLayer*)>;
 
class PopupLayer : public Layout {
public:
    CREATE_FUNC(PopupLayer);

    bool init(int width, int height, const Color3B& color, int opacity,
            bool enableTitle=true, const char* bgImage=nullptr);
    bool initWithModal(int width, int height, const Color3B& color, int opacity,
            bool enableTitle=true, const char* bgImage=nullptr);
     
    void addTitle(const char* title);
    void setClosedCallback(const dialogClosedCallback& callback);
    void setFont(const char* font, int size, float scale);
    void setTitle(const char* title);
    void setTitleBackgroundColor(const Color3B& color);
    int getTitleHeight() const;
    float getWidth() const;
    float getHeight() const;
    Size getMainPanelSize() const;

    void addMainPanel(Widget* widget, float margin=0.0f);

protected:
    virtual bool init() { return true; }
    bool _init(int width, int height, const Color3B& bgColor, int opacity,
            bool enableTitle, const char* bgImage);
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

    dialogClosedCallback _closedCallback = nullptr;
};

#endif  // _POPUP_LAYER_H_
