#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

struct PlayLayerHook : Modify<PlayLayerHook, PlayLayer> {
struct Fields {
	int m_latency;
	CCLabelBMFont* m_lLabel;
};
	bool init(GJGameLevel* level, bool useReplay, bool idk) {
		if (!PlayLayer::init(level, useReplay, idk)) return false;
		auto uilayer = this->m_uiLayer;
        m_fields->m_lLabel = CCLabelBMFont::create("", "chatFont.fnt");
        m_fields->m_lLabel->setPosition(ccp(uilayer->getContentSize().width - 30, uilayer->getContentSize().height - 5));
        m_fields->m_lLabel->setScale(0.5f);
        uilayer->addChild(m_fields->m_lLabel);
		return true;
	}


};

$register_ids(PlayLayer) {
    auto self = reinterpret_cast<PlayLayerHook*>(this);
}

class $modify(PlayerObjectHook, PlayerObject) {
    struct Fields {
        int m_latency;
        bool m_clicked;
        std::chrono::time_point<std::chrono::steady_clock> eventTime;
    };


    void pushButton(PlayerButton p0) {
        m_fields->eventTime = std::chrono::steady_clock::now();
        m_fields->m_clicked = true;

        PlayerObject::pushButton(p0);
    }

    void releaseButton(PlayerButton p0) {
        if (m_fields->m_clicked) {
            auto eventReleaseTime = std::chrono::steady_clock::now();
            m_fields->m_latency = std::chrono::duration_cast<std::chrono::milliseconds>(eventReleaseTime - m_fields->eventTime).count();
            m_fields->m_clicked = false;
            
            auto playLayer = static_cast<PlayLayerHook*>(PlayLayerHook::get());
            if (playLayer) {
                playLayer->m_fields->m_lLabel->setString(fmt::format("Latency: {}ms", m_fields->m_latency).c_str());
            }

        }

        PlayerObject::releaseButton(p0);
    }

    void update(float p0) {
        PlayerObject::update(p0);
    }
};