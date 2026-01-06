#pragma once

/**
 * @file BitwigBootContext.hpp
 * @brief Boot animation context - displays splash then transitions to Bitwig
 */

#include <memory>

#include <Arduino.h>
#include <lvgl.h>

#include <oc/context/IContext.hpp>
#include <oc/context/Requirements.hpp>
#include <oc/ui/lvgl/FontLoader.hpp>

#include "ui/font/BitwigFonts.hpp"
#include "ui/font/CoreFonts.hpp"
#include "ui/splash/SplashView.hpp"

namespace bitwig {

class BitwigBootContext : public oc::context::IContext {
public:
    static constexpr oc::context::Requirements REQUIRES{};

    bool initialize() override {
        // Load essential fonts for splash
        oc::ui::lvgl::font::loadEssential(CORE_FONT_ENTRIES, CORE_FONT_COUNT);

        splash_ = std::make_unique<ui::SplashView>(lv_screen_active());
        splash_->onActivate();
        splash_->setText("Connecting to Bitwig...");

        startMs_ = millis();
        return true;
    }

    void update() override {
        uint32_t elapsed = millis() - startMs_;

        // Start fade before end
        if (!fading_ && elapsed >= FADE_START_MS) {
            fading_ = true;
            splash_->fadeOut(FADE_MS, []() {});
        }

        // Switch when done
        if (elapsed >= DURATION_MS) {
            switchTo(1);  // BitwigContextID::BITWIG
        }
    }

    void cleanup() override {
        splash_.reset();
    }

    const char* getName() const override { return "BitwigBoot"; }

private:
    static constexpr uint32_t DURATION_MS = 1500;
    static constexpr uint32_t FADE_MS = 200;
    static constexpr uint32_t FADE_START_MS = DURATION_MS - FADE_MS;

    uint32_t startMs_ = 0;
    bool fading_ = false;
    std::unique_ptr<ui::SplashView> splash_;
};

}  // namespace bitwig
