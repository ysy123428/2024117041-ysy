const int ledPin = 2;

// 时间：短闪、长闪、间隔
const unsigned long DOT    = 250;
const unsigned long DASH   = 900;
const unsigned long GAP    = 250;
const unsigned long LETTER_GAP = 1000;
const unsigned long LOOP_GAP    = 3000;

unsigned long preTime = 0;
int step = 0;

// 一共 18步：S(6步) + O(6步) + S(6步)
#define MAX_STEP 18

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  unsigned long now = millis();

  if(now - preTime >= getTime(step)){
    preTime = now;
    step++;
    if(step >= MAX_STEP) step = 0;
  }

  digitalWrite(ledPin, isLight(step));
}

// 第几步要不要亮灯
bool isLight(int s){
  // 第一组S 三短
  if(s==0 || s==2 || s==4) return true;
  // 中间O 三长
  if(s==6 || s==8 || s==10) return true;
  // 最后S 三短
  if(s==12 || s==14 || s==16) return true;

  return false;
}

// 每一步的时间
unsigned long getTime(int s){
  // S 短亮
  if(s==0 || s==2 || s==4) return DOT;
  // O 长亮
  if(s==6 || s==8 || s==10) return DASH;
  // 最后S 短亮
  if(s==12 || s==14 || s==16) return DOT;

  // 字母之间大间隔
  if(s==5 || s==11) return LETTER_GAP;
  // 整套结束大停顿
  if(s==17) return LOOP_GAP;

  // 普通熄灭间隔
  return GAP;
}