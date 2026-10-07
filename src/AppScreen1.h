#ifndef APPSCREEN1_H
#define APPSCREEN1_H

#include <vector>
#include <string>
#include "imgui.h"

struct FileItem {
    std::string name;
    std::string type;
    std::string size;
    std::string dateModified;
    bool isDirectory;
};

class AppScreen1 {
public:
    AppScreen1();
    ~AppScreen1();

    void UpdateAndRender(bool* pOpen);

private:
    std::string m_currentPath;
    std::vector<FileItem> m_files;
    int m_selectedFileIndex;

    void LoadPath(const std::string& path);
};

#endif // APPSCREEN1_H
