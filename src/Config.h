#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

// A dummy process row loaded from config.txt ("process=PID,Name,CPU,MemMB,Status,User").
struct ProcessSeed {
    int pid;
    std::string name;
    float cpuUsage;
    float memoryUsageMB;
    std::string status;
    std::string user;
};

// Runtime parameters read from config.txt at startup, so values can be changed
// without recompiling. Anything missing from the file keeps the default below.
struct Config {
    // Window
    int windowWidth = 1280;
    int windowHeight = 720;
    std::string windowTitle = "CSOPESY Desktop OS Emulator";

    // Boot sequence
    bool bootEnabled = true;
    float biosDuration = 6.0f;      // seconds
    float loadingDuration = 5.0f;   // seconds
    bool allowSkip = true;          // Space / Enter / click skips a boot phase

    // Desktop
    int wallpaper = 0;              // 0 aurora, 1 cyberpunk, 2 midnight, 3 sunset
    bool clock24h = false;
    bool showSystemWidget = true;

    // Task Manager
    bool openTaskManagerOnStart = true;
    float telemetryInterval = 0.5f; // seconds between dummy value updates
    float totalRamGB = 16.0f;
    float baseMemUsedMB = 4500.0f;  // "system" memory on top of the process list
    std::vector<ProcessSeed> processes;

    std::string loadedFrom;         // path of the config file that was read ("" = defaults)

    static Config& Get();
    bool Load();                    // searches ./config.txt, ../config.txt, ../../config.txt
};

#endif // CONFIG_H
