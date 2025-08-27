#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include "include/Common.h"
#include <cstddef>

class LevelEditor
{
public:
    LevelEditor(const LevelEditor&) = delete;

    static LevelEditor& Get()
    {
        return s_Instance;
    }

    void DrawVertex();
    void DrawWindow();


private:
    LevelEditor() { }

    static LevelEditor s_Instance;



};

#endif
