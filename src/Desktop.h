#ifndef DESKTOP_H
#define DESKTOP_H

#include <string>
#include "imgui.h"

enum WallpaperStyle {
    WALLPAPER_AURORA = 0,
    WALLPAPER_CYBERPUNK,
    WALLPAPER_MIDNIGHT,
    WALLPAPER_SUNSET,
    WALLPAPER_COUNT
};

class Desktop {
public:
    Desktop();
    ~Desktop();

    void UpdateAndRender(bool* pwrShutdownRequested);
    void CycleWallpaper();
    std::string GetCurrentWallpaperName() const;

private:
    WallpaperStyle m_currentWallpaper;
    float m_animTime;

    void RenderWallpaper(ImDrawList* drawList, float width, float height);
    void RenderTopSystemBar(float width, bool* pwrShutdownRequested);
    void RenderDesktopIcons();
};

#endif // DESKTOP_H
