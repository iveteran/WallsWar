#ifndef _POPUP_MENU_H_
#define _POPUP_MENU_H_

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "PopupLayer.h"

USING_NS_CC;

using std::string;

namespace cocos2d::ui {
    class ListView;
}
using namespace cocos2d::ui;

class PopupMenuItem;

using clickMenuItemCallback = std::function<void(PopupMenuItem*, string&)>;

class PopupMenuItem : public Button {

public:
    CREATE_FUNC(PopupMenuItem);
    virtual bool init() override;

    void setClickedCallback(const clickMenuItemCallback callback);

private:
    clickMenuItemCallback _callback = nullptr;
};

class PopupMenuPanel : public PopupLayer {
public:
    CREATE_FUNC(PopupMenuPanel);
    virtual bool init() override;

    void addMenuItem(const char* name, const clickMenuItemCallback& clickedCallback);

private:
    ListView* _menuItems = nullptr;
};
 
class PopupMenu : public Layout {
public:
    CREATE_FUNC(PopupMenu);
    virtual bool init() override;

    void addMenuItem(const char* name, const clickMenuItemCallback& clickedCallback);

protected:
    void toggleMenuItems();
    void demoMenuItemClicked(PopupMenuItem* menuItem);
 
private:
    Button* _toggleBtn = nullptr;
    PopupMenuPanel* _popupMenuPanel;
};

#endif  // _POPUP_MENU_H_
