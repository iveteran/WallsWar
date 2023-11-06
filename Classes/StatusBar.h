#ifndef _STATUS_BAR_H_
#define _STATUS_BAR_H_

#include "cocos2d.h"
#include "CCVector.h"
#include "math/CCGeometry.h"
#include "ui/UILayout.h"

#include "Camp.h"

typedef std::function<void()> targetClickedCallback;

USING_NS_CC;

namespace cocos2d::ui {
    class Button;
    class Text;
    class PageView;
    class ImageView;
}
using namespace cocos2d::ui;

class ToggleButton;

class Player;
class Camp;


struct GameInfo {
    int frames = 0;
    int uptime = 0;
    int latency = 0;
    int blocks = 0;
    int players = 0;
    Size mapSize = Size::ZERO;

    Text* fpsLabel = nullptr;
    Text* uptimeLabel = nullptr;
    Text* networkLatencyLabel = nullptr;
    Text* playerNumberLabel = nullptr;
    Text* mapObjectNumberLabel = nullptr;
    Text* mapSizeLabel = nullptr;
    Text* windowSizeLabel = nullptr;

    Layout* container = nullptr;
};

struct PlayerInfo {
    //Text* nameLabel = nullptr;
    Text* scoreLabel = nullptr;
    Text* levelLabel = nullptr;
    Text* powerLabel = nullptr;
    Text* defenseLabel = nullptr;
    Text* positionLabel = nullptr;
    Text* rebornTimesLabel = nullptr;
    Text* rebornDelayLabel = nullptr;

    Player* player = nullptr;
    Layout* container = nullptr;
};

struct CampInfo {
    ImageView* campMarkIcon = nullptr;
    Text* campIndexLabel = nullptr;
    Text* nameLabel = nullptr;
    Text* scoreLabel = nullptr;
    Text* playerNumberLabel = nullptr;
    Text* positionLabel = nullptr;
    Text* playersRebornTimesLabel = nullptr;

    int index = 0;
    std::string name;
    Camp* camp = nullptr;
    Layout* container = nullptr;
};

struct CampsInfo {
    int campsTotal = 4;
    int currIndex = -1;
    int joinedIndex = 0;

    std::vector<CampInfo> items;
    PageView* container = nullptr;
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

    void setOpenSettingsCallback(const targetClickedCallback& callback);
    void setOpenMessagesBoxCallback(const targetClickedCallback& callback);

protected:
    void buildStatusGroups();
    void createExpandingToggleButton();
    void expandStatusBar(const ToggleButton* sender);
    void unexpandStatusBar(const ToggleButton* sender);
    void toggleExpandingStatusBar(const ToggleButton* sender);

    Layout* createInfoGroupLayout(const Color3B& bgColor,
        LinearLayoutParameter::LinearGravity alignMethod,
        const ccWidgetTouchCallback& callback);
    void setInfoGroupSizeAndBackground(Layout* layout, const Color3B& bgColor);

    void buildGameInfoGroup(Layout* parentLayout);
    void buildCampsInfoGroup(Layout* parentLayout);
    void buildPlayerInfoGroup(Layout* parentLayout);

    void buildSettingButtonsGroup(int numButtons);
    void createMessagesButton(Layout* parentLayout);
    void createServerPlayingButton(Layout* parentLayout);
    void createServerSettingsButton(Layout* parentLayout);
    void createSettingsButton(Layout* parentLayout);

    void pauseServerPlaying(const ToggleButton* sender);
    void startServerPlaying(const ToggleButton* sender);

    Text* createCustomizedLabel(const char* tag, const char* value, Layout* container);
    ImageView* createCustomizedIcon(const char* iconImg, Layout* container);
    void createSeperateLabel(Layout* container);

    virtual void update(float dt) override;

    void updateGameInfo();
    void updateFPS(bool addSep=true);
    void updateUptime(bool addSep=true);
    void updateNetworkLatency(bool addSep=true);
    void updatePlayersNumber(bool addSep=true);
    void updateMapObjectNumber(bool addSep=true);
    void showMapSize(bool addSep=true);
    void showWindowSize(bool addSep=true);

    void updatePlayerInfo();
    void updatePlayerScore(bool addSep=true);
    void updatePlayerLevel(bool addSep=true);
    void updatePlayerPower(bool addSep=true);
    void updatePlayerDefense(bool addSep=true);
    void updatePlayerPosition(bool addSep=true);
    void updatePlayerRebornTimes(bool addSep=true);
    void updatePlayerRebornDelay(bool addSep=true);

    void buildCampsInfoViewPages(PageView* pageView);
    void updateCampsInfo();
    void showCampMark(CampInfo& campInfo, bool addSep=true);
    void showCampIndexAndName(CampInfo& campInfo, bool addSep=true);
    void updateCampPlayers(CampInfo& campInfo, bool addSep=true);
    void updateCampPosition(CampInfo& campInfo, bool addSep=true);
    void updateCampScore(CampInfo& campInfo, bool addSep=true);
    void updateCampMemberRebornTimes(CampInfo& campInfo, bool addSep=true);

private:
    bool _useColorful;
    bool _expandingMode = false;

    float _buttonMargin;
    int _numInfoGroups = 0;
    int _numButtons = 0;

    float _accumDt = 0.0f;
    float _accumDt2 = 0.0f;

    GameInfo _gameInfo;
    PlayerInfo _playerInfo;
    CampsInfo _campsInfo;

    targetClickedCallback _openSettingsCallback = nullptr;
    targetClickedCallback _openMessagesBoxCallback = nullptr;
};

#endif  // _STATUS_BAR_H_
