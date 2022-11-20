#include "UIViewport.h"

namespace LevelEditor
{
    void UIViewport::Draw()
    {
        uint32_t backTexture = frameBuffer->GetTextureRenderID();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
        ImGui::Begin("Viewport");
        {
            ImVec2 vMin = ImGui::GetWindowContentRegionMin();
            ImVec2 vMax = ImGui::GetWindowContentRegionMax();
            vMin.x += ImGui::GetWindowPos().x;
            vMin.y += ImGui::GetWindowPos().y;
            vMax.x += ImGui::GetWindowPos().x;
            vMax.y += ImGui::GetWindowPos().y;
            ImGui::GetWindowDrawList()->AddImage((ImTextureID)backTexture, vMin, vMax, ImVec2(0,1), ImVec2(1,0));
            // Draw Gizmo
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(vMin.x, vMin.y, ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
            DrawGizmo();
        } 
        ImGui::End();
        ImGui::PopStyleVar();
    };

    void UIViewport::DrawGizmo()
    {
        
        if (uiSceneTree->mSelectedObject && mOperation != SelectGizmoEvent::GizmoOperation::None)
        {
            ImGuizmo::OPERATION op = (ImGuizmo::OPERATION)mOperation;
            auto& transform = uiSceneTree->mSelectedObject.GetComponent<Render::KitTransform>();

            /* Build transform matrix */
            float transformMatrix[16];
            ImGuizmo::RecomposeMatrixFromComponents(&transform.Translation.x, &transform.Rotation.x, &transform.Scale.x, transformMatrix);
            ImGuizmo::Manipulate(EditorCamera::Instance().GetView(), EditorCamera::Instance().GetPerspective(), uiTopBarTools->mode, ImGuizmo::MODE::LOCAL, transformMatrix);
            
            /* If we moved the manipulator */
            if (ImGuizmo::IsUsing())
            {
                /* We get new transformed components */
                float translation[3], rotation[3], scale[3];
                ImGuizmo::DecomposeMatrixToComponents(transformMatrix, translation, rotation, scale);

                if (op == ImGuizmo::OPERATION::TRANSLATE)
                {
                    transform.Translation = glm::vec3(translation[0], translation[1], translation[2]);
                }
                else if (op == ImGuizmo::OPERATION::ROTATE)
                {
                    glm::vec3 deltaRotation = glm::vec3(rotation[0], rotation[1], rotation[2]) - transform.Rotation;
                    transform.Rotation += deltaRotation;
                }
                else if (op == ImGuizmo::OPERATION::SCALE)
                {
                    transform.Scale = glm::vec3(scale[0], scale[1], scale[2]);
                }
            }
        }
    }
}