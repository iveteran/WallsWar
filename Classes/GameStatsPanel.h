#pragma once

#include "cocos2d.h"
#include "ui/UILayout.h"

USING_NS_CC;
using cocos2d::ui::Layout;

using clickedCallback = std::function<void (void)>;

class Camp;
class GameRuntime;

class GameStatsPanel : public Layout {
public:
    static GameStatsPanel* create(const GameRuntime* gameRuntime);

    void setClickedCallback(const clickedCallback& cb);
    void setClickedNextStageCallback(const clickedCallback& cb);

protected:
    bool init(const GameRuntime* gameRuntime);
    void createStatsTable();
    void createButtons();
    Layout* buildCampStatsCard(const Camp* camp, const Size& cardSize);

private:
    const GameRuntime* _gameRuntime = nullptr;
    clickedCallback _clickedCallback = nullptr;
    clickedCallback _clickedNextStageCallback = nullptr;
};

