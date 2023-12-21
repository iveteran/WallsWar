#pragma once

#include "cocos2d.h"

#include "ui/UILayout.h"

using std::string;

USING_NS_CC;

namespace cocos2d::ui {
    class Text;
    class ListView;
    class ImageView;
    class Button;
}
using namespace cocos2d::ui;

class StoryBlocks : public Ref {
public:
    CREATE_FUNC(StoryBlocks);
    virtual bool init();
};

class StoryWeapon : public Ref {
public:
    CREATE_FUNC(StoryWeapon);
    virtual bool init();
};

class StoryItems : public Ref {
public:
    CREATE_FUNC(StoryItems);
    virtual bool init();
};

class StoryMap : public Ref {
public:
    CREATE_FUNC(StoryMap);
    virtual bool init();
};

class StoryStage : public Ref {
public:
    CREATE_FUNC(StoryStage);
    virtual bool init();

private:
    int _id;
    string _name;
    string _description;
    StoryMap* _map;
};

class Story : public Ref {
public:
    CREATE_FUNC(Story);
    virtual bool init() { return true; }

    int getId() const { return _id; }

    const string& getName() const { return _name; }
    void setName(const string& name) { _name = name; }

    const string& getDescription() const { return _description; }
    void setDescription(const string& desc) { _description = desc; }

    const string& getCover() const { return _cover; }
    void setCover(const string& cover) { _cover = cover; }

    bool isSubscribed() { return _isSubscribed; }
    void setSubscribed() { _isSubscribed = true; }

private:
    int _id;
    string _name;
    string _description;
    string _cover;

    int _ownerId;
    string _owner;
    string _ownerEmail;
    time_t _createTime;

    bool _isSubscribed;

    Map<int, StoryStage*> _stages;

    Map<int, StoryBlocks*> _blocks;
    Map<int, StoryWeapon*> _weapons;
    Map<int, StoryItems*> _items;

    int _min_players;
    int _max_players;

    int _min_camps;
    int _max_camps;
};

enum class StoryCardEnv {
    None,
    PlayerStories,
    StoryMarket,
};

class StoryCard : public Layout {
public:
    static StoryCard* create(StoryCardEnv env, Story* story);
    bool init(StoryCardEnv env, Story* story);

private:
    ImageView* _bgImage = nullptr;
    Text* _summary = nullptr;
    Button* _activeBtn = nullptr;

    StoryCardEnv _env = StoryCardEnv::None;
    Story* _story = nullptr;
};
