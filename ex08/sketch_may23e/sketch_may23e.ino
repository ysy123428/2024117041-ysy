#include <WiFi.h>
#include <WebServer.h>

// 你的手机热点信息
const char* ssid = "Xiaomi 13";
const char* password = "401900ysy";

WebServer server(80);

// 引脚定义
#define TOUCH_PIN 4
#define LED_PIN 2

// 适配你的板子触摸逻辑（触碰数值升高）
#define TOUCH_TRIGGER 330

// 全局状态变量
bool systemArmed = false;  // 布防状态
bool isAlarmActive = false;// 报警锁定状态

// 网页主界面
void handleIndex() {
  String html = R"HTML(
  <html>
  <head>
    <meta charset="UTF-8">
    <title>安防报警主机</title>
    <style>
      body{text-align:center;margin-top:100px;font-size:24px;}
      button{padding:20px 50px;margin:20px;font-size:22px;cursor:pointer;}
    </style>
  </head>
  <body>
    <h1>ESP32 安防报警主机</h1>
    <button onclick="fetch('/arm')">布防 Arm</button><br>
    <button onclick="fetch('/disarm')">撤防 Disarm</button>
  </body>
  </html>
  )HTML";
  server.send(200, "text/html; charset=utf-8", html);
}

// 布防接口
void armSystem() {
  systemArmed = true;
  isAlarmActive = false;
  server.send(200, "text/plain; charset=utf-8", "✅ 系统已成功布防");
}

// 撤防接口
void disarmSystem() {
  systemArmed = false;
  isAlarmActive = false;
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/plain; charset=utf-8", "✅ 系统已撤防，报警已复位");
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);

  // 连接手机热点
  WiFi.begin(ssid, password);
  Serial.print("正在连接热点");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ 热点连接成功！");
  Serial.print("访问网页IP：");
  Serial.println(WiFi.localIP());

  // 绑定网页路由
  server.on("/", handleIndex);
  server.on("/arm", armSystem);
  server.on("/disarm", disarmSystem);
  server.begin();
}

void loop() {
  server.handleClient();
  int touchValue = touchRead(TOUCH_PIN);

  // 仅布防状态下，触碰引脚触发永久报警
  if (systemArmed && !isAlarmActive && touchValue >= TOUCH_TRIGGER) {
    isAlarmActive = true;
  }

  // 报警状态：LED高频狂闪
  if (isAlarmActive) {
    digitalWrite(LED_PIN, HIGH);
    delay(60);
    digitalWrite(LED_PIN, LOW);
    delay(60);
  }
}