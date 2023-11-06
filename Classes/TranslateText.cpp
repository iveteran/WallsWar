#include "cocos/platform/CCFileUtils.h"
#include "external/json/document.h"

static const char* _transFile = "text/zh_CN.json";
static rapidjson::Document _docJson;

bool _loadDocument(const char* transFile) {
    std::string fileData = cocos2d::FileUtils::getInstance()->getStringFromFile(transFile);
    if (_docJson.Parse(fileData.c_str()).HasParseError()) {
        CCLOG("Failed to load tranlation file: %s", transFile);
        return false;
    }
    return true;
}

const char* TEXT(const char* name) {
    if (_docJson.IsNull()) {
        _loadDocument(_transFile);
    }

    if (!_docJson.IsNull() && _docJson.HasMember(name)) {
        const char* value = _docJson[name].GetString();
        printf(">> TEXT %s -> %s\n", name, value);
        return value;
    }
    return name;
}
