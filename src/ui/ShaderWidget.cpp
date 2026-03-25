#include "ui/ShaderWidget.hpp"
#include <iostream>

namespace synth::ui {

ShaderWidget::ShaderWidget(std::string id, const std::string& fragShaderPath, const std::string& vertShaderPath)
    : Widget(std::move(id)) 
{
    //load shader
    if (vertShaderPath.empty()) {
        m_shader = LoadShader(nullptr, fragShaderPath.c_str());
    } else {
        m_shader = LoadShader(vertShaderPath.c_str(), fragShaderPath.c_str());
    }

    if (!IsShaderValid(m_shader)) {
        std::cerr << "ShaderWidget failed to load shader: " << fragShaderPath << std::endl;
    }

    m_timeLoc = GetShaderLocation(m_shader, "u_time");
    if (m_timeLoc == -1) m_timeLoc = GetShaderLocation(m_shader, "time");

    m_resolutionLoc = GetShaderLocation(m_shader, "u_resolution");
    if (m_resolutionLoc == -1) m_resolutionLoc = GetShaderLocation(m_shader, "resolution");

    m_renderTexture.id = 0; 
}

ShaderWidget::~ShaderWidget() {
    if (m_renderTexture.id != 0) {
        UnloadRenderTexture(m_renderTexture);
    }
    UnloadShader(m_shader);
}

void ShaderWidget::reallocateRenderTexture() {
    if (m_renderTexture.id != 0) {
        UnloadRenderTexture(m_renderTexture);
    }
    
    int width = static_cast<int>(m_bounds.width);
    int height = static_cast<int>(m_bounds.height);
    
    if (width > 0 && height > 0) {
        m_renderTexture = LoadRenderTexture(width, height);
    }
}

void ShaderWidget::computeLayout(Rectangle availableSpace) {
    bool sizeChanged = (m_bounds.width != availableSpace.width || m_bounds.height != availableSpace.height);
    Widget::computeLayout(availableSpace);
    
    if (sizeChanged && m_bounds.width > 0 && m_bounds.height > 0) {
        reallocateRenderTexture();
    }
}

void ShaderWidget::setParameter(const std::string& uniformName, std::function<float()> getter) {
    int loc = GetShaderLocation(m_shader, uniformName.c_str());
    if (loc != -1) {
        m_boundParams[uniformName] = {loc, std::move(getter)};
    } else {
        std::cerr << "ShaderWidget warning: Uniform '" << uniformName << "' not found for binding." << std::endl;
    }
}

void ShaderWidget::setUniformFloat(const std::string& uniformName, float value) {
    int loc = GetShaderLocation(m_shader, uniformName.c_str());
    if (loc != -1) {
        SetShaderValue(m_shader, loc, &value, SHADER_UNIFORM_FLOAT);
    }
}

void ShaderWidget::setUniformVec3(const std::string& uniformName, float x, float y, float z) {
    int loc = GetShaderLocation(m_shader, uniformName.c_str());
    if (loc != -1) {
        float vec[3] = {x, y, z};
        SetShaderValue(m_shader, loc, vec, SHADER_UNIFORM_VEC3);
    }
}

void ShaderWidget::update(float deltaTime) {
    if (!m_visible) return;

    m_runTime += deltaTime;

    //update
    if (m_timeLoc != -1) {
        SetShaderValue(m_shader, m_timeLoc, &m_runTime, SHADER_UNIFORM_FLOAT);
    }
    
    if (m_resolutionLoc != -1) {
        float resolution[2] = { m_bounds.width, m_bounds.height };
        SetShaderValue(m_shader, m_resolutionLoc, resolution, SHADER_UNIFORM_VEC2);
    }

    //pull and update all bound parameters
    for (const auto& [name, param] : m_boundParams) {
        float val = param.getter();
        SetShaderValue(m_shader, param.location, &val, SHADER_UNIFORM_FLOAT);
    }
}

void ShaderWidget::draw(const Theme& theme) const {
    if (!m_visible || m_renderTexture.id == 0 || m_bounds.width <= 0 || m_bounds.height <= 0) return;

 
    BeginTextureMode(m_renderTexture);
        ClearBackground(BLANK);
        BeginShaderMode(m_shader);
            DrawRectangle(0, 0, static_cast<int>(m_bounds.width), static_cast<int>(m_bounds.height), WHITE);
        EndShaderMode();
    EndTextureMode();


    Rectangle sourceRec = { 0.0f, 0.0f, static_cast<float>(m_renderTexture.texture.width), -static_cast<float>(m_renderTexture.texture.height) };
    DrawTextureRec(m_renderTexture.texture, sourceRec, {m_bounds.x, m_bounds.y}, WHITE);
}

} 
