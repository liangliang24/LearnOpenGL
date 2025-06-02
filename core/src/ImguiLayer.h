#pragma once
#include "Layer.h"

namespace LearnOpenGL
{
    class ImguiLayer :public Layer
    {
    public:
        ImguiLayer(const std::string& debugName);
        void Begin();
        void End();
        // Inherited via Layer
        void OnAttach() override;
        void OnDetach() override;
        void OnImguiRender() override;
        void OnUpdate(const Timestep& timestep) override;
        void OnEvent(Event& event) override;
    };
}

