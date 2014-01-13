# About the Project
KSynth aims to be (someday) a small software (using Qt) for composing songs using e.g. samplers and synthesizers using MIDI-Integreation for external devices like keyboards, a Launchpad, etc.
Additionally the code should work on several operating-systems such as Linux, Windows or maybe Mac OS.

# Structure
The code is distributed among two parts:
* **KSynth** is the backend (mainly header files) which can be used on its own. The backend contains samplers, synthesizers, generators, midi specific classes, audio outputs/exporters etc.
* **KSynthGUI** is the corresponding GUI wrapped around the backend for a userfriendly interface for composing songs.

Besides some well known libraries as tinyxml2 (https://github.com/leethomason/tinyxml2) and RtMidi (http://www.music.mcgill.ca/~gary/rtmidi/) the project also uses KLib which provides some helper functions for e.g. accessing files, streams, etc.


# External Dependencies
the external dependencies differ by your needs and your operating system. Currently the Qt-Project file sets some defines (like **-DWITH_FLAC**) to compile optional code using external libraries (here: **flac**) to provide additional functions (here: loading flac-compressed samples). 
* **linux:**
 * alsa
* **windows:**
 * winmm
* **optionals:**
 * flac (for compressed audio samples)
 * lame (for mp3 export, see: ToDo)


# Compiling
* If you simply need some way to e.g. load MIDI files and play them using synthesizers while adding some pattern-based drum loops within your code, simply go for the **KSynth** subfolder and add it to your project's include path.

* If you want to compose songs using the GUI, you ned booth, **KSynth** and **KSynthGUI** while the latter is just a Qt-Project and can be built using the **qtcreator**

Some parts of the code depend on my **KLib** library (https://github.com/k-a-z-u/KLib), currently providing some streaming APIs or file-access methods. While most of those methods were intended for another project, i decided to combine them within a single library to re-use them for other projects in the future.

The directory structure for compiling the GUI version should look as follows:

```
 |--ROOT
    |--KSynth           # backend (mainly header files, standalone)
    |--KSynthGUI        # Qt-Project, using above component
       |--lib
          |--KLib       # some helper functions. clone from https://github.com/k-a-z-u/KLib
          |--rtmidi     # accessing midi devices
          |--tinyxml2   # (de)serializing workspace, etc
```

Then just open the **KSynthGUI/KSynthGUI.pro**, hit F5 and have fun ;)

# ToDo
* As i just started working on this project a few weeks ago, there are still many things todo like fixing some horrible race-conditions in some areas ;)
* Most important parts are Widgets for displaying and modifing MIDI events (or, lets say: notes), adding the missing parts for loading/saving workspace, exporting your beautiful tracks to MP3 or Wave.
* Additional sound-outputs like Pulse, Core-Audio.
* Better Synthesizer(s)
* MIDI Integeration for event-binding between MIDI-Devices and the elements within your workspace
* And many many more :)

# License
Apache License Version 2.0
http://apache.org/licenses/LICENSE-2.0.txt
