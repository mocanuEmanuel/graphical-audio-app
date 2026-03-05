
- using Modern C++ (C++20) and Raylib for both graphics and audio

PROJECT STRUCTURE

- classic separation of headers and implementation C++ style
- build system is CMake, C++20 standard required, GLOB_RECURSE for source files


DESIGN PATTERNS USED
- Observer pattern — IObserver / ISubject interfaces in IObserver.hpp, StateManager is the Subject
  - observers vector with safeguards (duplicate check on add, erase-remove idiom on remove)
  - copy-on-iterate in notifyObserver to avoid invalidation if an observer modifies the list during notification
  - event enum (ParameterChanged, ModeChanged, EffectToggled) paired with std::any for flexible event data

- Strategy pattern — IEffect is the abstract interface for audio effects
  - applyEffect(std::span<float>) as the uniform processing contract
  - EffectBase intermediate class to avoid boilerplate (name, enabled flag, reset)
  - EffectFactory type alias (function pointer to factory) defined but not used yet
  - AudioEngine holds a vector of unique_ptr<IEffect> and chains them sequentially

- Command pattern — ICommand / CommandHistory for undo/redo
  - ICommand is the pure interface: execute(), undo(), getName()
  - merge support for consecutive small changes (like slider drags) — canMergeWith checks same target address, mergeWith absorbs the new value
  - CommandHistory manages dual stacks (undo/redo), clears redo on new action, has a configurable max size
  - callback hook (onHistoryChanged) for UI updates
  - (not wired into the app yet — sliders go straight to StateManager, no undo connected)

- Composite pattern — GridContainer is a Widget that contains other Widgets
  - tree structure: App owns a GridContainer root, which holds Slider children
  - computeLayout recursively distributes space down the tree
  - handleInput propagates in reverse order (top-most widget gets first shot)
  - draw delegates to all visible children

- RAII
  - App constructor calls InitWindow, destructor calls CloseWindow
  - AudioEngine constructor registers as observer + loads audio stream, destructor stops + unloads + deregisters
  - smart pointers manage lifetimes — unique_ptr for owned objects, shared_ptr for widgets in the grid
- Mediator StateManager aas central hub
  - all components talk through it instead of directly to each other
  - dual state: m_currentState (what's active) vs m_manuelState (what the user set via sliders) vs m_hardwareState (from Arduino, not populated yet)
  - mode switching logic: if mode is not HARDWARE, manual sets go through; when switching away from HARDWARE, restores last manual values


MODERN C++ ELEMENTS

- C++20 standard — set in CMake
- std::span<float> — used for audio buffer passing in IEffect and AudioEngine (bounds-safe view over contiguous memory, replaces raw float* + size_t)
- std::any — used in Observer events for type-erased data passing (temperature is float, mode is string, etc.)
- [[nodiscard]] — used consistently on all getters and query functions (getTemperature, canUndo, getName, etc.)
- lambda functions — used heavily for slider callbacks (setOnValueChanged captures this to call StateManager setters)
- std::function — stored as callbacks (onValueChanged in Slider, onHistoryChanged in CommandHistory)
- std::move — used for string parameters (Widget id, EffectBase name, command name) and unique_ptr transfers
- std::unique_ptr — default ownership model (effects in AudioEngine, StateManager and GridContainer in App, commands in history stacks)
- std::shared_ptr — used for widgets inside GridContainer (multiple references possible — grid owns them, but App also references during setup)
- std::make_unique / std::make_shared
- std::clamp — used in Slider::setValue and AudioEngine::setMasterVolume for bounds safety
- template class — ValueCommand<T> generalizes undo/redo for any value type
- dynamic_cast — used in ValueCommand::canMergeWith to check if another command targets the same variable
- static_cast — used for int/float conversions in UI drawing and audio buffer sizes
- enum class — Event type is a scoped enum (ParameterChanged, ModeChanged, EffectToggled)
- explicit constructors — used on single-argument constructors (Widget, EffectBase, CommandHistory, AudioEngine)
- structured bindings / range-for — used throughout for iterating vectors
- auto& — used for references to avoid copies (theme reference, command references)
- constexpr — INTERPOLATION_SPEED as a compile-time constant in StateManager
- #pragma once — used everywhere instead of include guards


NAMING CONVENTIONS

- PascalCase for classes and structs (StateManager, AudioEngine, GridContainer, SynthState)
- camelCase for methods and local variables (handleInput, computeLayout, getNormalizedValue)
- m_ prefix for private members (m_stateManager, m_audioBuffer, m_isDragging, m_effects)
- I prefix for interfaces (IObserver, ISubject, ICommand, IEffect)
- ALL_CAPS for constants (INTERPOLATION_SPEED)

FUNCTIONs
- Widget has some inline virtual defaults (empty update, false handleInput) — saves a .cpp file since base behavior is trivial
- ThemeManager is entirely inline/static — just returns a static theme, no .cpp needed
- (void) casts used to silence unused parameter warnings in not finished yet stuff


THE MAIN LOOP

- entry: main.cpp creates App(800, 450, "SynthApp [STARTUP]") and calls app.run()
- App::run() is a loop — while running && not window close: handleInput -> update(dt) -> render
- dt comes from Raylib's GetFrameTime()
- handleInput checks escape key + delegates to UI tree
- update propagates deltaTime to UI tree (and later will go to StateManager.update for interpolation)
- render grabs the theme, clears background, draws header text, draws the UI tree


STATEMANAGER

- SynthState struct holds: temperature, frequency, effectOn, lightLevel, humidity, mode
- three copies of state: m_currentState (active), m_manuelState (user/slider values), m_hardwareState (Arduino values — not populated yet)
- setters update m_manuelState first, then conditionally update m_currentState if not in HARDWARE mode
- toggleEffect and setMode always update regardless of mode
- setMode has transition logic: switching to OFF or MANUAL restores all manual values
- update(float deltaTime) not implemented yet
- INTERPOLATION_SPEED constant defined (5.0f) 


AUDIO SYSTEM

- AudioEngine takes a StateManager reference (dependency injection)
- registers itself as an observer in constructor, deregisters in destructor
- uses Raylib AudioStream for real-time custom audio streaming
- applyEffects iterates the effects vector calling applyEffect on each enabled one
- fillAudioStream checks IsAudioStreamProcessed before pushing new data(pull-based architecture)
- onNotify currently not implemented fully

UI 
- Widget is the abstract base (has id, bounds (Rectangle), visibility/enabled/focused/hovered flags)
- computeLayout is virtual (parent assigns bounds, child can override to do custom layout)
- draw takes a const Theme& (theming is passed down, not stored per widget)
- handleInput returns bool (consumed or not) (allows input to propagate or stop)
- GridContainer is the layout manager (rows x cols grid, divides space evenly)
  - addWidget stores a GridItem (widget + row + col position)
  - computeLayout calculates cell dimensions and adds 4px padding per cell
  - handleInput iterates children in reverse (last added = top-most = gets input first)
- Slider 
  - has min/max range, current value, label, dragging state
  - getNormalizedValue is a private helper for 0-to-1 mapping
  - handleInput does collision detection against the bar bounds (not the full widget bounds)
  - draw renders label, background bar, filled progress bar, numeric value text
  - hover/drag state changes the bar color (accent when active, primary when idle, textDim when disabled)
  - callback via std::function<void(float)> 
- ThemeManager is a static-only utility class 
  - getIndustrialTheme returns a static const Theme 
  - (only one theme defined so far, but the way i built supports adding more later)


SMART POINTER USAGE 
- unique_ptr: StateManager in App, GridContainer in App, IEffect instances in AudioEngine, ICommand instances in CommandHistory stacks
- shared_ptr: Widgets inside GridContainer (because addWidget takes shared_ptr (allows the caller to keep a reference during setup))
- raw pointers: IObserver* in the observers vector (nonowning reference ,observer lifetime managed elsewhere, probably will change)
- reference: StateManager& in AudioEngine (dependency injection — engine doesn't own the state manager)


WHAT'S IMPLEMENTED VS WHAT'S PLANNED BUT NOT DONE

- implemented:
  - full main loop with input -> update -> render cycle
  - StateManager with observer notification, dual-state (manual vs hardware), mode switching
  - IObserver/ISubject interfaces
  - ICommand + ValueCommand<T> template + CommandHistory with merge and undo/redo
  - AudioEngine skeleton (stream setup, effect chaining, start/stop)
  - IEffect interface + EffectBase shared implementation
  - Widget base class + Slider + GridContainer
  - ThemeManager with industrial theme
  - CMake build system with cross-platform support

- not implemented yet:
  - oscillator / actual sound generation (processAudio just fills silence default for now)
  - audio effects( until 15th march)
- need to connect CommandHistory to sliders(until 15th march)
  - Arduino integration(15th march onwards)
  - smoothing between manual and hardware states(after arduino implementation)
  - shader/visual effects (datamosh, CRT scanline, custom GLSL)(after 15th)
  - sample management / nature sounds (droplets, birds)(after 15th)
  - crossfading between audio sources(after 15th)
  - multiple themes or theme switching(after 15th)
  - tests


BUILD SETUP

- CMake 3.20+ required
- C++20 standard, strictly required
- raylib and nlohmann/json as git submodules in external/
- GLOB_RECURSE collects all .cpp files
- post-build step copies assets/ to build directory
- cross-platform linking: winmm on Windows, m/pthread/dl/rt/X11 on Linux


REFERENCES

- https://github.com/raysan5/raylib
- https://github.com/nlohmann/json
- https://www.raylib.com/cheatsheet/cheatsheet.html
- https://www.youtube.com/theaudioprogrammer
- https://www.researchgate.net/publication/242020079_The_Theory_and_Technique_of_Electronic_Music
- https://www.musicdsp.org/en/latest/
- https://www.earlevel.com/main/
- https://learnopengl.com/
- https://iquilezles.org/
- https://github.com/abaga129/lib_dsp
- https://github.com/jiwoojang/AudioVisualizer
- https://github.com/MrOneTwo/Raylib-shaders
- https://github.com/raysan5/raylib/blob/master/examples/shaders/shaders_model_shader.c