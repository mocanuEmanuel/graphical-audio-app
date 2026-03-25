#pragma once

#include <string>
#include <memory>
#include <functional> 

namespace synth {

class ICommand {
public:
    virtual ~ICommand() = default;
    
    virtual void execute() = 0;

    virtual void undo() = 0;

    [[nodiscard]] virtual std::string getName() const = 0;


    [[nodiscard]] virtual bool canMergeWith(const ICommand& other) const {
        return false;
    }

    virtual bool mergeWith(const ICommand& other) {
        return false;
    }
};


template<typename T>
class StateCommand : public ICommand {
public:
    using Setter = std::function<void(T)>;
    using Getter = std::function<T()>;

    StateCommand(std::string name, Getter getter, Setter setter, T newValue)
        : m_name(std::move(name))
        , m_setter(setter)
        , m_oldValue(getter())
        , m_newValue(newValue)
    {}

    void execute() override {
        m_setter(m_newValue);
    }

    void undo() override {
        m_setter(m_oldValue);
    }

    [[nodiscard]] std::string getName() const override {
        return m_name;
    }

    // Merge consecutive slider movements into a single Undo step
    [[nodiscard]] bool canMergeWith(const ICommand& other) const override {
        auto* otherCmd = dynamic_cast<const StateCommand<T>*>(&other);
        return otherCmd && otherCmd->m_name == m_name;
    }

    bool mergeWith(const ICommand& other) override {
        auto* otherCmd = dynamic_cast<const StateCommand<T>*>(&other);
        if (otherCmd && otherCmd->m_name == m_name) {
            m_newValue = otherCmd->m_newValue; // Overwrite end value
            return true;
        }
        return false;
    }

private:
    std::string m_name;
    Setter m_setter;
    T m_oldValue;
    T m_newValue;
};

} 
