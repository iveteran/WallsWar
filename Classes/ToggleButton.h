#ifndef _TOGGLE_BUTTON_H_
#define _TOGGLE_BUTTON_H_

#include "cocos2d.h"
#include "ui/UIButton.h"

namespace cocos2d::ui {
    class Layout;
}
using namespace cocos2d::ui;
using std::string;

class ToggleButton;

using cbToggleButtonClicked = std::function<void(ToggleButton*)>;

class ToggleButton : public Button {

public:
    //CREATE_FUNC(ToggleButton);
    //virtual bool init() override;

    static ToggleButton* create(const string& onButtonImage, const string& offButtonImage,
        const cbToggleButtonClicked& toggleOnCallback, const cbToggleButtonClicked& toggleOffCallback,
        bool isOn=false);

    void setOn(bool value);

protected:
    bool init(const string& onButtonImage, const string& offButtonImage,
        const cbToggleButtonClicked& toggleOnCallback, const cbToggleButtonClicked& toggleOffCallback,
        bool isOn=false);
    void onClicked();

private:
    bool _isOn = false;
    string _onButtonImage;
    string _offButtonImage;
    cbToggleButtonClicked _toggleOnCallback = nullptr;
    cbToggleButtonClicked _toggleOffCallback = nullptr;
};

#endif  // _TOGGLE_BUTTON_H_
