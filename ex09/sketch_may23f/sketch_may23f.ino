#include <WiFi.h>
#include <WebServer.h>

// 你的手机热点信息
const char* ssid = "Xiaomi 13";
const char* password = "401900ysy";

WebServer server(80);
#define TOUCH_PIN 4

// 网页仪表盘界面
void handleIndex() {
  String html = R"HTML(
  <html>
  <head>
    <meta charset="UTF-8">
    <title>触摸实时仪表盘</title>
    <style>
      body{text-align:center;margin-top:120px;}
      h1{font-size:32px;}
      #data{font-size:130px;color:red;font-weight:bold;}
    </style>
  </head>
  <body>
    <h1>触摸传感器 实时监控仪表盘</h1>
    <div id="data">0</div>
    <script>
      // AJAX每200ms自动拉取、更新数值
      setInterval(()=>{
        fetch("/getSensor")
        .then(res => res.text())
        .then(value => {
          document.getElementById('data').innerText = value;
        });
      }, 200);
    </script>
  </body>
  </html>
  )HTML";
  server.send(200, "text/html; charset=utf-8", html);
}

// 接口：返回当前触摸传感器原始数值
void returnSensorData() {
  int currentValue = touchRead(TOUCH_PIN);
  server.send(200, "text/plain; charset=utf-8", String(currentValue));
}

void setup() {
  Serial.begin(115200);

  // 连接热点
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("✅ 连接完成");
  Serial.print("仪表盘访问IP：");
  Serial.println(WiFi.localIP());

  // 绑定路由
  server.on("/", handleIndex);
  server.on("/getSensor", returnSensorData);
  server.begin();
}

void loop() {
  server.handleClient();
}