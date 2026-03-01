#include "core/CommandHistory.hpp"

namespace synth {

CommandHistory::CommandHistory(size_t maxSize)
    : m_maxSize(maxSize)
{
}

void CommandHistory::executeCommand(std::unique_ptr<ICommand> command, bool merge) {
    if (!command) return;
    
    //try to merge with previous command
    if (merge && !m_undoStack.empty()) {
        auto& lastCommand = m_undoStack.back();
        if (lastCommand->canMergeWith(*command)) {
            if (lastCommand->mergeWith(*command)) {
                //merged successfully, rexecute the merged command
                lastCommand->execute();
                m_redoStack.clear();
                if (m_onHistoryChanged) m_onHistoryChanged();
                return;
            }
        }
    }

    command->execute();
    
    //add to undo stack
    m_undoStack.push_back(std::move(command));
    
    //clear redo stack (new action invalidates redo history(can change later))
    m_redoStack.clear();
    
    //trim history if needed(exceed max size(maybe not good to call like this and 
    //should do a check like how arrays update doubling and stuff))
    trimHistory();
    
    if (m_onHistoryChanged) m_onHistoryChanged();
}

bool CommandHistory::undoCommand() {
    if (m_undoStack.empty()) return false;
    
    //pop from undo stack
    auto command = std::move(m_undoStack.back());
    m_undoStack.pop_back();
  
    command->undo();
    
    //push to redo stack
    m_redoStack.push_back(std::move(command));
    
    if (m_onHistoryChanged) m_onHistoryChanged();
    return true;
}

bool CommandHistory::redoCommand() {
    if (m_redoStack.empty()) return false;
    
    //pop from redo stack
    auto command = std::move(m_redoStack.back());
    m_redoStack.pop_back();
    
    // rexecute
    command->execute();
    
    //push to undo stack
    m_undoStack.push_back(std::move(command));
    
    if (m_onHistoryChanged) m_onHistoryChanged();
    return true;
}

bool CommandHistory::canUndoCommand() const {
    return !m_undoStack.empty();
}

bool CommandHistory::canRedoCommand() const {
    return !m_redoStack.empty();
}

std::string CommandHistory::getUndoCommandName() const {
    if (m_undoStack.empty()) return "";
    return m_undoStack.back()->getName();
}

std::string CommandHistory::getRedoCommandName() const {
    if (m_redoStack.empty()) return "";
    return m_redoStack.back()->getName();
}

void CommandHistory::clearHistory() {
    m_undoStack.clear();
    m_redoStack.clear();
    if (m_onHistoryChanged) m_onHistoryChanged();
}

size_t CommandHistory::getUndoCommandCount() const {
    return m_undoStack.size();
}

size_t CommandHistory::getRedoCommandCount() const {
    return m_redoStack.size();
}

void CommandHistory::setOnHistoryChanged(std::function<void()> callback) {
    m_onHistoryChanged = std::move(callback);
}

void CommandHistory::trimHistory() {
    //maybe remove more at a time idk i think it would be better
    while (m_undoStack.size() > m_maxSize) {
        m_undoStack.erase(m_undoStack.begin());
    }
}

} 
