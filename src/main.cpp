#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;


class $modify(PlayerObjectHook, PlayerObject) {
    struct Fields {
        int m_latency;
        bool m_clicked;
        std::chrono::time_point<std::chrono::steady_clock> eventTime;
    };

    void pushButton(PlayerButton p0) {
        // Capture the event time when the button is pressed
        m_fields->eventTime = std::chrono::steady_clock::now();
        m_fields->m_clicked = true;

        // Call the original pushButton method
        PlayerObject::pushButton(p0);
    }

    void releaseButton(PlayerButton p0) {
        if (m_fields->m_clicked) {
            // Measure the latency when the button is released
            auto eventReleaseTime = std::chrono::steady_clock::now();
            m_fields->m_latency = std::chrono::duration_cast<std::chrono::milliseconds>(eventReleaseTime - m_fields->eventTime).count();
            log::info("Latency: {}ms", m_fields->m_latency);
            m_fields->m_clicked = false;
        }

        // Call the original releaseButton method
        PlayerObject::releaseButton(p0);
    }

    void update(float p0) {
        // Ensure the player updates correctly
        PlayerObject::update(p0);
    }
};