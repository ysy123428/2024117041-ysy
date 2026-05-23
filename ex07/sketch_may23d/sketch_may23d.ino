#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Xiaomi 13";
const char* password = "401900ysy";

WebServer server(80);
const int LED_PIN = 2;

void handleRoot(){
  String html = R"HTML(
  <html>
  <head>
    <meta charset="UTF-8">
    <title>ESP32 无极调光器</title>
  </head>
  <body style="text-align:center;margin-top:100px">
    <h1>ESP32 无极灯光调光器</h1>
    <input type="range" min="0" max="255" id="slider" style="width:90%">
    <p>当前亮度：<span id="value">0</span></p >
    <script>
      const s = document.getElementById('slider');
      const v = document.getElementById('value');
      s.oninput = ()=>{
        v.innerText = s.value;
        fetch("/set?b="+s.value);
      }
    </script>
  </body>
  </html>
  )HTML";
  server.send(200,"text/html; charset=utf-8",html);
}

void handleBright(){
  int brightness = server.arg("b").toInt();
  analogWrite(LED_PIN, constrain(brightness,0,255));
  server.send(200,"text/plain","OK");
}

void setup(){
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);

  WiFi.begin(ssid,password);
  Serial.print("正在连接热点...");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ 连接成功！访问IP：");
  Serial.println(WiFi.localIP());

  server.on("/",handleRoot);
  server.on("/set",handleBright);
  server.begin();
}

void loop(){
  server.handleClient();
}