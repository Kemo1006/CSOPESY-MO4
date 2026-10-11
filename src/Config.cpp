#include "Config.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

static std::string Trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) ++a;
    while (b > a && isspace((unsigned char)s[b - 1])) --b;
    return s.substr(a, b - a);
}

static std::string Lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (char)tolower(c); });
    return s;
}

static bool ParseBool(const std::string& v, bool fallback) {
    std::string s = Lower(v);
    if (s == "1" || s == "true" || s == "yes" || s == "on") return true;
    if (s == "0" || s == "false" || s == "no" || s == "off") return false;
    return fallback;
}

static float ParseFloat(const std::string& v, float fallback, float lo, float hi) {
    char* end = nullptr;
    float f = strtof(v.c_str(), &end);
    if (end == v.c_str()) return fallback;
    return std::max(lo, std::min(hi, f));
}

static int ParseInt(const std::string& v, int fallback, int lo, int hi) {
    char* end = nullptr;
    long n = strtol(v.c_str(), &end, 10);
    if (end == v.c_str()) return fallback;
    return (int)std::max((long)lo, std::min((long)hi, n));
}

static int ParseWallpaper(const std::string& v, int fallback) {
    std::string s = Lower(v);
    if (s == "aurora") return 0;
    if (s == "cyberpunk") return 1;
    if (s == "midnight") return 2;
    if (s == "sunset") return 3;
    return ParseInt(v, fallback, 0, 3);
}

Config& Config::Get() {
    static Config instance;
    return instance;
}

bool Config::Load() {
    const char* candidates[] = { "config.txt", "../config.txt", "../../config.txt" };
    std::ifstream in;
    for (const char* path : candidates) {
        in.open(path);
        if (in.is_open()) { loadedFrom = path; break; }
        in.clear();
    }
    if (!in.is_open()) {
        fprintf(stderr, "config.txt not found - using built-in defaults.\n");
        return false;
    }

    std::string line;
    int lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;
        line = Trim(line);
        if (line.empty() || line[0] == '#') continue;

        size_t eq = line.find('=');
        if (eq == std::string::npos) {
            fprintf(stderr, "config.txt:%d: ignored (no '=')\n", lineNo);
            continue;
        }
        std::string key = Lower(Trim(line.substr(0, eq)));
        std::string val = Trim(line.substr(eq + 1));

        if (key == "window_width")              windowWidth = ParseInt(val, windowWidth, 640, 7680);
        else if (key == "window_height")        windowHeight = ParseInt(val, windowHeight, 480, 4320);
        else if (key == "window_title")         windowTitle = val;
        else if (key == "boot_enabled")         bootEnabled = ParseBool(val, bootEnabled);
        else if (key == "bios_duration")        biosDuration = ParseFloat(val, biosDuration, 0.5f, 120.0f);
        else if (key == "loading_duration")     loadingDuration = ParseFloat(val, loadingDuration, 0.5f, 120.0f);
        else if (key == "allow_skip")           allowSkip = ParseBool(val, allowSkip);
        else if (key == "wallpaper")            wallpaper = ParseWallpaper(val, wallpaper);
        else if (key == "clock_24h")            clock24h = ParseBool(val, clock24h);
        else if (key == "show_system_widget")   showSystemWidget = ParseBool(val, showSystemWidget);
        else if (key == "open_task_manager")    openTaskManagerOnStart = ParseBool(val, openTaskManagerOnStart);
        else if (key == "telemetry_interval")   telemetryInterval = ParseFloat(val, telemetryInterval, 0.05f, 10.0f);
        else if (key == "total_ram_gb")         totalRamGB = ParseFloat(val, totalRamGB, 1.0f, 4096.0f);
        else if (key == "base_mem_used_mb")     baseMemUsedMB = ParseFloat(val, baseMemUsedMB, 0.0f, 4194304.0f);
        else if (key == "process") {
            // PID,Name,CPU,MemMB,Status,User
            std::vector<std::string> f;
            std::stringstream ss(val);
            std::string tok;
            while (std::getline(ss, tok, ',')) f.push_back(Trim(tok));
            if (f.size() < 4) {
                fprintf(stderr, "config.txt:%d: process needs PID,Name,CPU,MemMB[,Status,User]\n", lineNo);
                continue;
            }
            ProcessSeed p;
            p.pid = ParseInt(f[0], 0, 0, 99999999);
            p.name = f[1];
            p.cpuUsage = ParseFloat(f[2], 0.0f, 0.0f, 100.0f);
            p.memoryUsageMB = ParseFloat(f[3], 0.0f, 0.0f, 4194304.0f);
            p.status = f.size() > 4 && !f[4].empty() ? f[4] : "Running";
            p.user = f.size() > 5 && !f[5].empty() ? f[5] : "SYSTEM";
            processes.push_back(p);
        }
        else fprintf(stderr, "config.txt:%d: unknown key '%s'\n", lineNo, key.c_str());
    }
    fprintf(stderr, "Loaded %s\n", loadedFrom.c_str());
    return true;
}
