#include "guistate.h"
#include <gui.h>
#include <imgui.h>


void GuiState::Update()
{
    ImGui::Begin("logger", &loggerOpen);
    {
        ImGui::BeginChild("scrolling");
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
        //if (copy) ImGui::LogToClipboard();

        /*
				if (Filter.IsActive())
				{
					const char* buf_begin = Buf.begin();
					const char* line = buf_begin;
					for (int line_no = 0; line != NULL; line_no++)
					{
						const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
						if (Filter.PassFilter(line, line_end))
							ImGui::TextUnformatted(line, line_end);
						line = line_end && line_end[1] ? line_end + 1 : NULL;
					}
				}
				else
				*/
        {
            ImGui::TextUnformatted(logger.c_str());
        }

        ImGui::SetScrollHere(1.0f);

        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::End();
    }
}

