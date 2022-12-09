#include "UIViewport.h"

namespace LevelEditor
{
    void UIViewport::Draw()
    {
        uint32_t backTexture = frameBuffer->GetTextureRenderID();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3.0f,3.0f));
        
        ImGui::Begin("Viewport");
        {
            ImVec2 vMin = ImGui::GetWindowContentRegionMin();
            ImVec2 vMax = ImGui::GetWindowContentRegionMax();
            vMin.x += ImGui::GetWindowPos().x;
            vMin.y += ImGui::GetWindowPos().y;
            vMax.x += ImGui::GetWindowPos().x;
            vMax.y += ImGui::GetWindowPos().y;
            //ImGui::GetWindowDrawList()->AddImage((ImTextureID)backTexture, vMin, vMax, ImVec2(0,1), ImVec2(1,0));
            ImGui::Image((ImTextureID)backTexture, ImGui::GetContentRegionAvail(), ImVec2(0,1), ImVec2(1,0));

            if(ImGui::BeginDragDropTarget())
            {
                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Item_content_browser"))
                {
                    const wchar_t* path = (const wchar_t*)payload->Data;
                }
                ImGui::EndDragDropTarget();
            }

            // Draw Gizmo

            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(vMin.x, vMin.y, vMax.x - vMin.x, vMax.y - vMin.y);
            DrawGizmo();
        } 
        ImGui::End();
        ImGui::PopStyleVar();
    };

    void UIViewport::DrawGizmo()
    {
        const auto& world = Render::GameLevel::Get();
        GameObject* pSelectedEntity = world.GetSelectedEntity();
        if (pSelectedEntity && mOperation != GizmoOperation::NONE)
        {
            glm::vec3 position = pSelectedEntity->GetPosition();
            glm::vec3 rotation = glm::degrees(pSelectedEntity->GetRotation());
            glm::vec3 scale = pSelectedEntity->GetScale();

            /* Build transform matrix */
            float transformMatrix[16];
            ImGuizmo::RecomposeMatrixFromComponents(&position.x, &rotation.x, &scale.x, transformMatrix);
            ImGuizmo::Manipulate(EditorCamera::Instance().GetView(), EditorCamera::Instance().GetPerspective(), (ImGuizmo::OPERATION)mOperation, ImGuizmo::MODE::LOCAL, transformMatrix);
            
            /* If we moved the manipulator */
            if (ImGuizmo::IsUsing())
            {
                ImGuizmo::DecomposeMatrixToComponents(transformMatrix, &position.x, &rotation.x, &scale.x);
                switch (mOperation)
                {
                case GizmoOperation::TRANSLATE:
                    pSelectedEntity->SetPosition(position);
                    break;
                case GizmoOperation::ROTATE:
                    {
                        pSelectedEntity->SetRotationInDegrees(rotation);
                    }
                    break;
                case GizmoOperation::SCALE:
                    pSelectedEntity->SetScale(scale);
                    break;
                default:
                    break;
                }
            }
        }
    }
}