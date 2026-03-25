#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include "raylib.h"
#include "ui/Widget.hpp"

namespace synth::ui {

class ShaderWidget : public Widget {
public:
    ShaderWidget(std::string id, const std::string& fragShaderPath, const std::string& vertShaderPath = "");
    ~ShaderWidget() override;

    ShaderWidget(const ShaderWidget&) = delete;
    ShaderWidget& operator=(const ShaderWidget&) = delete;

    void update(float deltaTime) override;
    void draw(const Theme& theme) const override;
    void computeLayout(Rectangle availableSpace) override;

 
    void setParameter(const std::string& uniformName, std::function<float()> getter);
    void setUniformFloat(const std::string& uniformName, float value);
    void setUniformVec3(const std::string& uniformName, float x, float y, float z);

private:
    void reallocateRenderTexture();

    Shader m_shader;
    RenderTexture2D m_renderTexture;
    
    //passed uniforms
    int m_timeLoc = -1;
    int m_resolutionLoc = -1;
    float m_runTime = 0.0f;

    //custom dynamic parameters bound to external state
    struct BoundParam {
        int location;
        std::function<float()> getter;
    };
    std::unordered_map<std::string, BoundParam> m_boundParams;
};

}