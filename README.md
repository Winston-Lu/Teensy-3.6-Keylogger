# Teensy-3.6-Keylogger
A crude key-logger made on a Teensy 3.6 using the Arduino IDE.



## Note:

Modifier keys such as Shift, Ctrl, Alt are only meant to be emulated for typing, and not for uses such as gaming. These keys are held down or released only when a non-modifier key is pressed due to how the USBHost_t36 library handles the OnPress() and OnRelease() functions. The crude implementation is meant to support unicode characters when using alt-codes. As of the current version, alt-codes do not seem to work.

Other modifier keys such as the GUI (Windows) key will not work unless paired with a non-modifier key, such as GUI+D. Pressing the GUI key by itself will not work.

Other keys such as the function keys, caps/num/scroll locks, as well as the numpad keys should work as usual. 

Media keys such as those on a Corsair K95 are untested, and is not supported at this moment
