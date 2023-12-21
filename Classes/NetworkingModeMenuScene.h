#pragma once

#include "cocos2d.h"
#include "ui/UILayout.h"

#include "NetworkingMode.h"

USING_NS_CC;

namespace cocos2d::ui {
    class Text;
    class Button;
}
using namespace cocos2d::ui;

class CardsView;
class NetworkingModeCard;
class UISceneTitlePanel;

using onSelectedCallback = std::function<void(NetworkingModeCard*)>;
using onEnteringCallback = std::function<void(NetworkingModeCard*)>;

class NetworkingModeCard : public Layout {
public:
    static NetworkingModeCard* create(const char* name,
            const char* bgImage, const char* summary);
    bool init(const char* name, const char* bgImage, const char* summary);

    const std::string& getSummary() const { return _summary; }
    void setSelectedCallback(const onSelectedCallback& cb);
    void setEnteringCallback(const onEnteringCallback& cb);
    void setActive();
    void setUnactive();

private:
    std::string _title;
    std::string _summary;
    onSelectedCallback _onSelectedCallback = nullptr;
    onEnteringCallback _onEnteringCallback = nullptr;
    Button* _enterBtn = nullptr;
};

class NetworkingModeMenuScene : public Scene {
public:
    CREATE_FUNC(NetworkingModeMenuScene);
    virtual bool init() override;

protected:
    CardsView* buildNMCards(float width, float height);
    Layout* buildSummaryPanel(float width, float height);

    void onItemSelected(NetworkingModeCard* nmCard);
    void onEnterSingleMode(NetworkingModeCard* nmCard);
    void onEnterLocalMode(NetworkingModeCard* nmCard);
    void onEnterOnlineMode(NetworkingModeCard* nmCard);
    void onHistoryButtonClicked(const UISceneTitlePanel* titlePanel);

private:
    CardsView* _nmCards = nullptr;
    NetworkingModeCard* _selectedNMCard = nullptr;
    Layout* _summaryPanel = nullptr;
    Text* _summaryLabel = nullptr;
};
