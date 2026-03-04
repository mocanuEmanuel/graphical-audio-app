#pragma once
#include "Widget.hpp"
#include <vector>
#include <memory>

namespace synth::ui {
//handles placement of widgets
class GridContainer : public Widget {
public:
    GridContainer(std::string id, int rows, int cols)
        : Widget(std::move(id)), m_rows(rows), m_cols(cols) {}

    void addWidget(std::shared_ptr<Widget> widget, int row, int col) {
        if (widget) m_children.push_back({std::move(widget), row, col});
    }

    void update(float deltaTime) override;
    void draw(const Theme& theme) const override;
    bool handleInput() override;
    
    //slice m_bounds into a matrix, assign bounds to children
    //maybe i shouldnt and keep it simpler
    void computeLayout(Rectangle availableSpace) override;

private:
    struct GridItem {
        std::shared_ptr<Widget> widget;
        int row;
        int col;
    };
    
    int m_rows;
    int m_cols;
    std::vector<GridItem> m_children;
};
}