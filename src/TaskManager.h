#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <vector>
#include <string>
#include "imgui.h"

struct ProcessInfo {
    int pid;
    std::string name;
    float cpuUsage;
    float memoryUsageMB;
    std::string status;
    std::string user;
};

class TaskManager {
public:
    TaskManager();
    ~TaskManager();

    void UpdateAndRender(bool* pOpen);

private:
    std::vector<ProcessInfo> m_processes;
    int m_selectedProcessIndex;
    float m_updateTimer;
    float m_baseMemMB;
    float m_usedMemMB;
    
    // Dynamic performance plot history
    std::vector<float> m_cpuHistory;
    std::vector<float> m_ramHistory;

    void InitializeProcesses();
    void UpdateTelemetry(float deltaTime);
    void RenderProcessesTab();
    void RenderPerformanceTab();
    void RenderSystemSummaryTab();
};

#endif // TASKMANAGER_H
