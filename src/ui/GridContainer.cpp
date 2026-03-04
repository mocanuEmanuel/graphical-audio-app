#include "ui/GridContainer.hpp"

namespace synth::ui {

void GridContainer::update(float deltaTime) {
    if (!m_visible || !m_enabled) return;
    
    for (auto& child : m_children) {
        child.widget->update(deltaTime);
    }
}

void GridContainer::draw(const Theme& theme) const {
    if (!m_visible) return;
    for (const auto& child : m_children) {
        child.widget->draw(theme);
    }
}

bool GridContainer::handleInput() {
    if (!m_visible || !m_enabled) return false;
    
    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
        if (it->widget->handleInput()) {
            return true;
        }
    }
    return false;
}

void GridContainer::computeLayout(Rectangle availableSpace) {
    Widget::computeLayout(availableSpace); // sets m_bounds
    
    if (m_rows <= 0 || m_cols <= 0) return;
    
    float cellWidth = availableSpace.width / static_cast<float>(m_cols);
    float cellHeight = availableSpace.height / static_cast<float>(m_rows);

    for (auto& child : m_children) {
        Rectangle childSpace = {
            availableSpace.x + (child.col * cellWidth),
            availableSpace.y + (child.row * cellHeight),
            cellWidth,
            cellHeight
        };
        
        float pad = 4.0f; 
        childSpace.x += pad;
        childSpace.y += pad;
        childSpace.width -= (pad * 2.0f);
        childSpace.height -= (pad * 2.0f);

        child.widget->computeLayout(childSpace);
    }
}

}