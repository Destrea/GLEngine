
//Level Editor cpp file

//Utilizes a canvas and Dear ImGui, it formats a text file that can be used to generate level-data
#include "../include/LevelEditor.h"
#include "../include/ImguiLayer.h"

//Grab the attribute data from the LevelLayout, to use/change while editing the level in the editor.
LevelEditor LevelEditor::s_Instance;
static ImVector<LevelEditor::EditorVertex> vertices;
static ImVector<LevelEditor::EditorEdge> edges;
static LevelEditor::EditorVertex nullVert;
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
static std::vector<std::string> ListItems;
int Create_Edge(LevelEditor::EditorEdge);

//Add "draw_list" static pointer, for use with Create_Edge


//Add funtionality of pushing the new points/vertices onto an array, and formatting them to work like vertex attribute data.




float round_to_grid(int n, int GRID_STEP)
{
    int a = GRID_STEP * (n/GRID_STEP);
    int b = GRID_STEP * (n/GRID_STEP) + GRID_STEP;

    return (n - a >= b - n)? b : a;
}


LevelEditor::EditorVertex* LevelEditor::getVertInfo(int vertID)
{
    for(int i = 0; i < vertices.size(); i++)
    {
        if(vertices[i].ID == vertID)
        {
            return &vertices[i];
        }
    }

    return &nullVert;
}


int checkVertAtPos(ImVec2 hoveredLoc)
{
    for(int i = 0; i < vertices.size(); i++)
    {
        if(vertices[i].location == hoveredLoc)
        {
            return vertices[i].ID;
        }
    }

    return -1;
}

void LevelEditor::DrawWindow()
{
    if(ImGui::Begin("Canvas"))
    {

        //General Options and variables
        static ImVector<ImVec2> points;
        static ImVec2 scrolling(0.0f,0.0f);
        static bool opt_enable_grid = true;
        static bool opt_enable_context_menu = true;
        static bool adding_line = false;
        static bool adding_vert = false;

        const char* items[] = {"Vertex", "Line", "Move", "None"};
        static int item_selected_idx = 0;
        //Options for grid-snapping
        const float GRID_STEP = 32.0f;

        static ImGuiColorEditFlags base_flags = ImGuiColorEditFlags_None;







        //Settings checkboxes
        ImGui::Checkbox("Enable grid", &opt_enable_grid); ImGui::SameLine();
        ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
        ImGui::Combo("Draw Mode", &item_selected_idx, items, IM_ARRAYSIZE(items)); ImGui::SameLine();
        if(ImGui::Button("Add Edge"))
        {
            ImGui::OpenPopup("Adding Edge");
        }

        if(ImGui::BeginPopupModal("Adding Edge", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            static int item_selected_idx1 = 0;
            static ImGuiComboFlags flags = 0;
            const char* combo_preview_value1 = ListItems[item_selected_idx1].c_str();

            if(ImGui::BeginCombo("Vertex 1", combo_preview_value1, flags))
            {
                int len = vertices.size();


                for (int n = 0; n < vertices.size(); n++)
                {
                    const bool is_selected = (item_selected_idx1 == n);
                    if (ImGui::Selectable(ListItems[n].c_str(), is_selected))
                        item_selected_idx1 = n;
                    if(is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }

            static int item_selected_idx2 = 0;
            const char* combo_preview_value2 = ListItems[item_selected_idx2].c_str();

            if(ImGui::BeginCombo("Vertex 2", combo_preview_value2, flags))
            {
                int len = vertices.size();

                for (int n = 0; n < vertices.size(); n++)
                {
                    const bool is_selected = (item_selected_idx2 == n);
                    if (ImGui::Selectable(ListItems[n].c_str(), is_selected))
                        item_selected_idx2 = n;
                    if(is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }

            //Add confirmation box
            if(ImGui::Button("Add"))
            {
                LevelEditor::EditorVertex* v1 = getVertInfo(item_selected_idx1);
                LevelEditor::EditorVertex* v2 = getVertInfo(item_selected_idx2);
                LevelEditor::EditorEdge tempEdge;
                int edgeID = edges.size() + 1;
                std::stringstream ss;
                ss << "EdgeID: " << edgeID;
                std::string edgeName = ss.str();
                tempEdge.Init(edgeID, v1, v2, edgeName);
                edges.push_back(tempEdge);
                ImGuiManager::Get().addLogItem("info", "Edge Created.");
            }

            if(ImGui::Button("Close"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }



        //ImGui::Text("Selected Opton: %d",item_selected_idx);
        //ImGui::Text("Color Settings:");
        static ImVec4 v_color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
        //ImGui::ColorEdit3("Vertex Color", (float*)&v_color, base_flags);

        ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
        ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
        if(canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
        if(canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
        ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);


        //Draw Canvas Border and Background
        ImGuiIO& io = ImGui::GetIO();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50,50,50,255));
        draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255,255,255,255));

        //Interaction catching
        ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
        const bool is_hovered = ImGui::IsItemHovered();
        const bool is_active = ImGui::IsItemActive();
        const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y);  //Lock scrolled origin
        const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);


        //Line-drawing mouse handling
        //Add first and second point
        if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            if(item_selected_idx == 2)
            {
                ImVec2 mouseGridPos(round_to_grid(mouse_pos_in_canvas.x, GRID_STEP), round_to_grid(mouse_pos_in_canvas.y, GRID_STEP));
                int status = checkVertAtPos(mouseGridPos);
                if(status == -1)
                {
                    ImGuiManager::Get().addLogItem("error", "No vertex found at this location.");
                }
                else
                {
                    selectedObj = status;
                    std::stringstream ss;
                    ss << "Vertex with ID: " << status << " found at this location.";
                    std::string message = ss.str();
                    ImGuiManager::Get().addLogItem("info", message);
                }
            }
            else if(item_selected_idx == 1)
            {
                points.push_back(mouse_pos_in_canvas);
                points.push_back(mouse_pos_in_canvas);
                adding_line = true;
            }
            //Drawing a vertex
            else if(item_selected_idx == 0)
            {

                ImVec2 location(round_to_grid(mouse_pos_in_canvas.x, GRID_STEP), round_to_grid(mouse_pos_in_canvas.y, GRID_STEP));
                EditorVertex tempVert;
                int vertID = vertices.size() + 1;
                std::stringstream ss;
                ss << "VertID: " << vertID;
                std::string vertName = ss.str();

                tempVert.Init(vertID, location, vertName);
                vertices.push_back(tempVert);
                ListItems.push_back(tempVert.name);
                adding_vert = true;
            }
        }
        if (adding_line)
        {
            points.back() = mouse_pos_in_canvas;
            if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                adding_line = false;
        }
        if (adding_vert)
        {
            if(!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                adding_vert = false;
        }

        //Pan the canvas around
        const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
        if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
        {
            scrolling.x += io.MouseDelta.x;
            scrolling.y += io.MouseDelta.y;
        }


        //Right Click context menu
        ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
        if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
        {
            ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
        }
        if(ImGui::BeginPopup("context"))
        {
            if(adding_line)
                points.resize(points.size() -2);
            if(adding_vert)
                vertices.resize(vertices.size() -1);
            adding_line = false;
            adding_vert = false;
            if(ImGui::MenuItem("Remove one", NULL, false, (points.Size > 0 || vertices.Size > 0) )) { points.resize(points.size() -2); vertices.resize(vertices.size() - 1);}
            if(ImGui::MenuItem("Remove all", NULL, false, (points.Size > 0 || vertices.Size > 0))) { points.clear(); vertices.clear(); }
            ImGui::EndPopup();

        }

        //Draw Canvas GridLines/dots, as well as all the lines.
        draw_list->PushClipRect(canvas_p0, canvas_p1, true);
        if(opt_enable_grid)
        {
            for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
                draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
            for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
                draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
        }
        for(int n = 0; n < points.Size; n+=2)
        {
            draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n+1].x, origin.y + points[n+1].y), IM_COL32(255,255,0,255), 2.0f);
        }
        for(int v = 0; v < vertices.size(); v++)
        {
            draw_list->AddCircleFilled(ImVec2(origin.x + vertices[v].location.x, origin.y + vertices[v].location.y), 12.0f, IM_COL32((float)v_color.x * 255.0f, (float)v_color.y * 255.0f, (float)v_color.z * 255.0f, 255), 16);
        }


        draw_list->PopClipRect();

    }
    ImGui::End();


}


void Create_Edge(LevelEditor::EditorEdge* edgeVar)
{
    draw_list->Add
}



