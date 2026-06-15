#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class ConsolePopup : public geode::Popup<std::string> {
protected:
    TextInput* m_input;

    bool setup(std::string const&) override {
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // Заголовок
        auto title = CCLabelBMFont::create("Console", "bigFont.fnt");
        title->setPosition(winSize.width / 2, winSize.height - 50);
        title->setScale(0.7f);
        m_mainLayer->addChild(title);

        // Поле ввода
        m_input = TextInput::create(200, 30, "Type command", "console");
        m_input->setPosition(winSize.width / 2, winSize.height - 100);
        m_input->setCallback([this](std::string const& cmd) {
            executeCommand(cmd);
        });
        m_mainLayer->addChild(m_input);

        // Кнопка закрытия
        auto closeBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"),
            this,
            menu_selector(ConsolePopup::onClose)
        );
        closeBtn->setPosition(winSize.width - 30, winSize.height - 30);
        auto menu = CCMenu::create();
        menu->addChild(closeBtn);
        m_mainLayer->addChild(menu);

        return true;
    }

    void executeCommand(std::string const& cmd) {
        if (cmd == "help") {
            log::info("Commands: help, fps, close");
        } 
        else if (cmd.find("fps") == 0) {
            if (cmd == "fps") {
                int current = Mod::get()->getSettingValue<int>("fps-limit");
                log::info("Current FPS limit: {}", current);
            } else {
                // Парсим "fps 240"
                int value = 240;
                try {
                    value = std::stoi(cmd.substr(4));
                } catch (...) {}
                Mod::get()->setSettingValue<int>("fps-limit", value);
                log::info("FPS limit set to {}", value);
            }
        }
        else if (cmd == "close") {
            this->onClosePopup(nullptr);
        }
        else {
            log::info("Unknown command: {}", cmd);
        }

        // Очищаем поле ввода
        m_input->setString("");
    }

public:
    static ConsolePopup* create() {
        auto ret = new ConsolePopup();
        if (ret && ret->initAnchored(300, 200)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        // Кнопка для открытия консоли
        auto btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_garageBtn_001.png"),
            this,
            menu_selector(MenuLayer::onConsole)
        );

        auto menu = CCMenu::create();
        menu->addChild(btn);
        menu->setPosition(50, 50);
        this->addChild(menu);

        return true;
    }

    void onConsole(CCObject*) {
        ConsolePopup::create()->show();
    }
};
