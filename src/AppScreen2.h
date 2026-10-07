#ifndef APPSCREEN2_H
#define APPSCREEN2_H

#include <vector>
#include <string>
#include "imgui.h"

class AppScreen2 {
public:
    AppScreen2();
    ~AppScreen2();

    void UpdateAndRender(bool* pOpen);

private:
    char m_inputBuffer[256];
    std::vector<std::string> m_history;
    bool m_scrollToBottom;

    void ExecuteCommand(const char* cmd);
    void AddLog(const char* fmt, ...);
};

#endif // APPSCREEN2_H
