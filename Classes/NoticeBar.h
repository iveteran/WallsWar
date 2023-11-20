#ifndef _NOTICE_BAR_H
#define _NOTICE_BAR_H

#include "ui/UILayout.h"

using std::string;

USING_NS_CC;

namespace cocos2d::ui {
    class Button;
    class Text;
    class ImageView;
    class PageView;
}
using namespace cocos2d::ui;

class NoticeItem;

using removeNoticeCallback = std::function<void(NoticeItem*)>;

enum class NoticeLevel {
    NONE,
    INFO,
    NOTICE,
    WARNING,
    ERROR,
};

Color3B getMessageColor(NoticeLevel level);

class NoticeItem : public Layout {
public:
    static NoticeItem* create(NoticeLevel level, const string& msg,
            const removeNoticeCallback& removingCb);
    bool init(NoticeLevel level, const string& msg,
            const removeNoticeCallback& removingCb);

    int getId() { return _id; }

private:
    int _id;
    removeNoticeCallback _removingCb = nullptr;

    NoticeLevel _level = NoticeLevel::NONE;
    ImageView* _levelIcon = nullptr;
    Text* _message = nullptr;
    Button* _removingBtn = nullptr;
};

class NoticeBar : public Layout {
public:
    CREATE_FUNC(NoticeBar);
    bool virtual init() override;

    void setRemovedCallback(const removeNoticeCallback& cb);
    int addNotice(const string& msg, NoticeLevel level); // return id
    void removeNotice(int id);
    bool empty() const;
    int getSize() const;

protected:
    void onRemovingClicked(NoticeItem* noticeItem);
    void onNoticeRemoved();

    void rollNotice();
    virtual void update(float dt) override;

private:
    PageView* _container = nullptr;
    removeNoticeCallback _noticeRemovedCb = nullptr;

    int _currIndex = -1;
    float _accumDt = 0.0f;
};

#endif // _NOTICE_BAR_H
