#define LED1 2
#define LED2 16

void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop()
{
  // 灯1渐亮，灯2同步渐暗
  for(int bri = 0; bri <= 255; bri++)
  {
    analogWrite(LED1, bri);
    analogWrite(LED2, 255 - bri);
    delay(6);
  }
  // 灯1渐暗，灯2同步渐亮
  for(int bri = 255; bri >= 0; bri--)
  {
    analogWrite(LED1, bri);
    analogWrite(LED2, 255 - bri);
    delay(6);
  }
}