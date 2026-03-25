
WHO USES IT

- new-media artists and installation designers
- sound designers and experimental musicians
- creative-coding students and educators
- HCI enthusiasts?more here but i think i got something
- DIY / maker community
- common thread: interested in the connection between the enviroment and the sounds that it could produce


WHAT NEED IT FILLS

- gap between expensive pro tools (TouchDesigner, Max/MSP) and bare-bones frameworks (Processing, openFrameworks)
- real-time integrated(big apps usually have plugins or setups u need to do) sensor-to-media mapping: Arduino sensors -> StateManager    -> AudioEngine + ShaderWidget
- unified audio + visual engine in one app (no separate tools for sound and image, apps like SuperCollider are audio only)
- low-cost but fulfilling hands on experience


MARKET POSITION

- somewhere between pro av tools like TouchDesigner and basic frameworks
- growing interest could be driven by immersive installations, arduino hands on enjoyers, data sonification interest
- positioning: open-source, sensor-nativity, for people passionate of mixed art mediums


WHY ITS INTERESTING 

- being present, aware, maybe a demonstration of how chaotic and unpleasant the sounds and visuals get when you would insert parameters that would appear in a natural disaster (climate change)

- The immersive, multi-sensory nature of the app, being enveloped in sound and visuals that respond to the same environment you're physically in: growing demand from museums, festivals, brands

- Although it doesn't have AI, the AI craze has made many people interested in systems that generate audiovisual experiences reacting to input. Someone that wants to explore a similar concept but not as black-box-like as an AI from a corporation, or doesn't want to deal with the huge undertaking that is machine learning, may be interested in playing around with this app to see what they can create. And unlike AI that creates from a massive data pool, they can follow the methodology I set up that dictates the relation between the input and output.

- DIY accessability,Arduino + cheap sensors

- modern C++ in creative coding is not seen as often since many people that may be interested in creating such tools are coming from a artist background more so than a programmer one, so languages like python are more acccessible and common, but they leave space for improvement in terms of performance and engineering design choices.


FOCUS POINTS

1  Room-level installation
- mine is small-scale, personal
- contrast with server-scale installations that may call upon big wheather information servers


2  Sound-environment mapping and psychology
- which sounds fit which environmental parameters?
- moody gray humid day vs sunny hot day, different sound palettes
- academic stuff to research further for this information and extract: cross-modal correspondence, ecological psychoacoustics
- need psychology papers on emotion-sensory environment connection
- look into soundscape ecology (Pijanowski), embodied cognition


4 New media artists / social aspect / memorable experiences
- multi-sensory experiences are remembered more vividly(source later)
- social experience,being in a shared reactive environment
- choose artists to reference


5 Extendability and DIY
- swapping sensors
- adding effects
- adding shaders
- adding sound samples to overlay on the generated sound

6 Technical aspects of the code itself(pull from DevNotes i got a ton of stuff already piled up on that, easy part)
- raylib
- modern C++20
- cmake
- arduino integration
- design patterns
- cross platform (windows, linux, mac)