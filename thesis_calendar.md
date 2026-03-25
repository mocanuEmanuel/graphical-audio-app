THESIS CALENDAR


#### WEEK 1 — Mar 25 – Mar 31

-APP — SHADERS
- halftone, grain shaders etc

-SOUND DESIGN
- research cross-modal correspondence papers to start making mapping decisions: which sounds/timbres for which environmental states
- document initial mapping decisions

-ARTICLE / RESEARCH
- tie more sources to claims
- begin writing introduction
- reference section
- categorize references by topic

-APP — ARDUINO / IOT
- connect logic to device
- test manual-hardware switching mode

-APP — SAVE/LOAD
- implement saving states
- add keyboard shortcut (Ctrl+S / Ctrl+L) or UI button for save/load


#### WEEK 2 — Apr 1 – Apr 7

-ARTICLE / RESEARCH
- write System Architecture chapter (design patterns, C++20 elements, RAII, smart pointer usage)
- include architecture diagram
- write brief methodology section (what design decisions were made and why)

-APP — SHADER REFINEMENT
- tweak existing shaders to respond more expressively to StateManager parameters
- experiment with shader parameter range (visual changes are noticeable and aesthetically coherent)
- add at least one more artistic shader (options: watercolor wash, pixel sort, or voronoi pattern)

-SOUND DESIGN
- decide on 2–3 environmental sound scapes/palettes
- begin choosing waveform + frequency + modulation ranges for each palette
- research basic music theory timing (BPM or rhythmic pulsing tied to sensor cadence)

-APP — AUDIO
- add 1–2 more IEffect subclasses (maybe DelayEffect, BitcrushEffect) to enrich the audio palette
- wire new effects to StateManager events (e.g., humidity → delay feedback, light → bitcrush depth)


#### WEEK 3 — Apr 8 – Apr 14

-APP — ARDUINO INTEGRATION POLISH
- add error handling, sensor calibration, stable serial baud rate
- handle connection drops, reconnect logic, invalid data handling
- implement StateManager::update() interpolation for smooth hardware→current transitions

-SOUND DESIGN + AUDIO
- adding sample playback overlay (e.g., ambient nature sounds: rain, wind, birds) mixed with oscillator output
- create SamplePlayer class, load WAV files from assets/audio/, mix into AudioEngine::processAudio()
- group sounds by music theory timing: define note durations relative to BPM, align oscillator frequency changes to beat subdivisions

-ARTICLE
- screenshots
- record a short demo video (30 seconds) showing sensor -> audio + visual reaction


#### WEEK 4 — Apr 15 – Apr 21

-APP — INTEGRATION AND POLISH
- full end-to-end test: Arduino sensors → SerialReader → StateManager → AudioEngine + ShaderWidgets
- test save/load with all new parameters (effects, shader choices, palettes)
- UI tweaks: integrate tooltip correct info
- bug fixing pass


#### WEEK 5 — Apr 22 – Apr 28

-ARTICLE
- abstract ready by now in draft
- format bibliography

-APP — LATER LOW PRIORITY FEATURES
- multiple themes or theme switching
- crossfading between audio sources
- final visual polish on shaders, colour palette consistency, smooth transitions
