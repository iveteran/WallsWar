#pragma once

#include "cocos2d.h"
#include "ui/UILayout.h"

USING_NS_CC;
namespace cocos2d {
    class Label;
}
namespace cocos2d::ui {
    class Button;
    class EditBox;
}
using namespace cocos2d::ui;

class SearchBar;

using searchingCallback = std::function<void(const SearchBar*, const char*)>;
using gettingTopsCallback = std::function<void(const SearchBar*, int topNum)>;

class SearchBar : public Layout {
public:
    static const int WIDTH = 150;
    static const int HEIGHT = 20;

public:
    static SearchBar* create(float width=WIDTH, float height=HEIGHT);
    bool init(float width, float height);

    void setSearchingCallback(const searchingCallback& cb);
    void setGettingTopNCallback(const gettingTopsCallback& cb, int topN);

protected:
    searchingCallback _searchingCb = nullptr;
    gettingTopsCallback _gettingTopsCb = nullptr;

private:
    int _topN = 10;
    EditBox* _inputField = nullptr;
    Label* _topNBtnLabel = nullptr;
    Button* _topNBtn = nullptr;
};
