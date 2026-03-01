#pragma once

#include <string>
#include <memory>

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
class ValueCommand : public ICommand {
public:
    ValueCommand(std::string name, T& target, T newValue)
        : m_name(std::move(name))
        , m_target(target)
        , m_oldValue(target)
        , m_newValue(newValue)
    {}
    
    void execute() override {
        m_target = m_newValue;
    }

    void undo() override {
        m_target = m_oldValue;
    }

    [[nodiscard]] std::string getName() const override {
        return m_name;
    }
    

    //merge functions for merging consecutive small value changes like sliders
    [[nodiscard]] bool canMergeWith(const ICommand& other) const override {
        auto* otherValue = dynamic_cast<const ValueCommand<T>*>(&other);
        return otherValue && &otherValue->m_target == &m_target;
    }

    bool mergeWith(const ICommand& other) override {
        auto* otherValue = dynamic_cast<const ValueCommand<T>*>(&other);
        if (otherValue && &otherValue->m_target == &m_target) {
            m_newValue = otherValue->m_newValue;
            return true;
        }
        return false;
    }

protected:
    std::string m_name;
    T& m_target;
    T m_oldValue;
    T m_newValue;
};

} 
