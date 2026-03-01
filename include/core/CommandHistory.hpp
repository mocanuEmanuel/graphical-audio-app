#pragma once

#include "ICommand.hpp"
#include <vector>
#include <memory>
#include <functional>

namespace synth {

//undo/redo manager
class CommandHistory {
public:
    //maybe change this to a HARD limit later at the top
    explicit CommandHistory(size_t maxHistorySize = 100);
    ~CommandHistory() = default;
    
    //execute and add to history(maybe i should split this into two functions later)
    //merge true -> try to merge with previous command
    void executeCommand(std::unique_ptr<ICommand> command, bool merge = false);
   
    bool undoCommand();
    
    bool redoCommand();
    
    [[nodiscard]] bool canUndoCommand() const;
    
    
    [[nodiscard]] bool canRedoCommand() const;
    
     //command name to be undone
    [[nodiscard]] std::string getUndoCommandName() const;
    
    //command name to be redone
    [[nodiscard]] std::string getRedoCommandName() const;
    
    
    void clearHistory();
    
    //history size
    [[nodiscard]] size_t getUndoCommandCount() const;
    [[nodiscard]] size_t getRedoCommandCount() const;
    
        //set callback for when history changes
    void setOnHistoryChanged(std::function<void()> callback);

private:
    void trimHistory();
    
    std::vector<std::unique_ptr<ICommand>> m_undoStack;
    std::vector<std::unique_ptr<ICommand>> m_redoStack;
    size_t m_maxSize;
    std::function<void()> m_onHistoryChanged;
};

} 
