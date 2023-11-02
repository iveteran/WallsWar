#ifndef _STATUS_BAR_H_
#define _STATUS_BAR_H_

#include "cocos2d.h"
#include "CCVector.h"
#include "math/CCGeometry.h"
#include "ui/UILayout.h"

#include "Camp.h"

typedef std::function<void()> openSettingsCallback;

USING_NS_CC;

namespace cocos2d::ui {
    class Button;
    class Text;
}
using namespace cocos2d::ui;

class ToggleButton;

class Player;
class Camp;


struct GameInfo {
    float frames = 0.0f;
    float fps = 0.0f;

    int uptime = 0;
    int latency = 0;
    int blocks = 0;
    int players = 0;
    Size mapSize = Size::ZERO;

    Text* fpsLabel = nullptr;
};

struct CampsInfo {
    Vector<Camp*> camps;

    int currIndex;
};
 
class StatusBar : public Layout {
public:
    static const bool EXPANDING = true;
    static const bool UNEXPANDING = false;
    static const bool COLORFUL = true;
    static const bool UNCOLORFUL = false;

public:
    CREATE_FUNC(StatusBar);

    virtual bool init() override;
    bool init(bool expandingMode, bool colorful);
    bool init(int width, int height, const Color3B& bgColor, int opacity, const char* bgImage=nullptr);

    void setOpenSettingsCallback(const openSettingsCallback& callback);

protected:
    void buildStatusGroups();
    void createExpandingToggleButton();
    void expandStatusBar(const ToggleButton* sender);
    void unexpandStatusBar(const ToggleButton* sender);
    void toggleExpandingStatusBar(const ToggleButton* sender);

    Layout* createInfoGroupLayout(const Color3B& bgColor,
        LinearLayoutParameter::LinearGravity alignMethod,
        const ccWidgetTouchCallback& callback);
    void buildGameInfoGroup(Layout* parentLayout);
    void buildCampsInfoGroup(Layout* parentLayout);
    void buildPlayerInfoGroup(Layout* parentLayout);

    void buildSettingButtonsGroup(int numButtons);
    void createServerPlayingButton(Layout* parentLayout);
    void createServerSettingsButton(Layout* parentLayout);
    void createSettingsButton(Layout* parentLayout);

    void pauseServerPlaying(const ToggleButton* sender);
    void startServerPlaying(const ToggleButton* sender);

    virtual void update(float dt) override;

private:
    bool _useColorful;
    bool _expandingMode = false;

    float _buttonMargin;
    int _numInfoGroups = 0;
    int _numButtons = 0;

    float _accumDt = 0.0f;

    GameInfo _gameInfo;
    CampsInfo _campsInfo;

    Player* _player = nullptr;

    openSettingsCallback _openSettingsCallback = nullptr;
};

#endif  // _STATUS_BAR_H_
