#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include "Common.h"
#include <cstddef>
#include <sstream>
class LevelEditor
{
public:
    LevelEditor(const LevelEditor&) = delete;

    static LevelEditor& Get()
    {
        return s_Instance;
    }

    void DrawWindow();
    int Level_Export(char *fileName);
    int Level_Import(char *fileName);


    struct EditorVertex
    {

        void Init(int ID_In, ImVec2 location_In, std::string name_In) {
            ID = ID_In;
            location = location_In;
            name = name_In;
        }
        std::string name = "VertexN";
        int ID = -1;
        ImVec2 location = ImVec2(0,0);
    };
    struct EditorEdge
    {

        void Init(int ID_In, LevelEditor::EditorVertex* vert1_In, LevelEditor::EditorVertex* vert2_In, std::string edgeName) {
            ID = ID_In;
            vert1 = vert1_In;
            vert2 = vert2_In;
        }
        int ID;
        LevelEditor::EditorVertex* vert1;     //Uses the ID numbers of the vertices as the references
        LevelEditor::EditorVertex* vert2;     //Uses the ID numbers of the vertices as the references
        float height = 1.0f;
        //texture

    };

    int selectedObj;
    EditorVertex* getVertInfo(int vertID);
    bool addEdge;

private:
    LevelEditor() { }

    static LevelEditor s_Instance;
    //int checkVertAtPos(ImVec2 hoveredLoc);  //Returns the vertex's ID if it can find a vertex at that location, returns -1 if it can't.


};

#endif
