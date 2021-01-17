# Teensy-3.6-Keylogger
A key-logger for a US-Layout keyboard made for a Teensy 3.6 using the Arduino IDE.

This keylogger code works an nearly indetectable keylogger. Actions such as key presses and released are mimicked, as well as modifier keys such as Alt, Ctrl, Shift, etc. Other keyloggers often just type a key when a key is pressed, which will not work if they need to hold keys down, such as when you are working on spreadsheets, documents, or other computer programs.

# Modifications
This project is based on the USBHost_t36.h library, which I had to make a small, yet crude modification to get it to work.

I modified the keyboard.cpp file in /USBHost_t36/keyboard.cpp, where I commented out lines 256, 265, 275, and 283. Those lines detect if a attachRawReleasefunction or attachOnRawPress function is defined. Those lines seem to not detect if the function is actually defined, so I jerry rigged a temporary solution for it.

If you wish to adapt this code for a different keyboard layout, feel free to fork this repo or suggest any changes. You are free to modify or use this code however you want.

# Version 1 vs Version 2
Version 1 is based on a function that detects if a key is pressed. Unfortunately, this does not activate when modifier keys such as CTRL are pressed. I have rewritten this for V2 where the use of a different detection function makes this keylogger a lot more undetectable.

# Limitations and Current Issues
On my computer, scroll lock activates caps lock, caps lock does nothing, and num lock is inverted (based on the LED). This does not seem to exhibit the same behaviour on other systems. This may be due to my keyboard being rebound (Caps lock -> Esc for vim).

I have only tested this code for the English US language with the ANSI 101/104 layout. Any other keyboard layout is still untested, but would work for 99% of keyboards in NA.

Looking into the source code, some keys such as right control and left control are defined as the same key. This is a limitation in Arduino's software itself, so this may pose a problem where binds are used heavily, such as in games or macro-heavy workloads.

# To Do
Find if there is a code for specifically left/right CTRL and such

