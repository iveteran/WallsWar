#ifndef _STACKED_BUTTON_H_
#define _STACKED_BUTTON_H_

#include "cocos2d.h"
#include "ui/UIButton.h"

using namespace cocos2d::ui;
using std::string;

class StackedButton;

using clickedCallback = std::function<void(StackedButton*, int)>;

class StackedButton : public Button {
public:
    struct Item {
        string icon;
        clickedCallback callback;
        int id;
        void* userData;
    };

public:
    static StackedButton* create(const std::vector<Item>& items);

    int getCurrentIndex() const;
    const Item* getCurrentItem() const;
    const Item* getItem(int index) const;

    void select(int index);

protected:
    bool init(const std::vector<Item>& items);
    void onClicked();

    void shift();
    void replaceIcon();

private:
    std::vector<Item> _items;
    int _currentIndex = 0;
};

#endif  // _STACKED_BUTTON_H_
