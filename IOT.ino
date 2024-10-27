// ĐỘNG CƠ DC
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Khai báo các chân điều khiển động cơ và LED
const int motorPin1 = D6; // Chân IN1 của mạch điều khiển động cơ
const int motorPin2 = D5; // Chân IN2 của mạch điều khiển động cơ
const int ledPin1 = D1;   // Chân kết nối LED 1
const int ledPin2 = D2;   // Chân kết nối LED 2
const int ledPin3 = D3;   // Chân kết nối LED 3

ESP8266WebServer server(80); // Khởi tạo server trên cổng 80

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Kết nối WiFi
  WiFi.begin("OPPOReno", "vanmanh999"); // SSID và mật khẩu
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connecting...");
  }
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());

  // Thiết lập các chân điều khiển động cơ và LED là output
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  // Thiết lập server
  server.on("/", [] {
    server.send(200, "text/html",
                "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<title>Control DC Motor and LED</title>"
                "<script>"
                "function sendOption() {"
                "var optionInput = document.getElementById('option');"
                "var xhttp = new XMLHttpRequest();"
                "var option = optionInput.value;"
                "xhttp.open('POST', 'sendOption', true);"
                "xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');"
                "xhttp.send('option=' + option);"
                "}"
                "</script>"
                "</head>"
                "<body>"
                "<label>Option (1: Engine On, 2: Engine Off, 3: LED1 On, 4: LED1 Off, 5: LED2 On, 6: LED2 Off, 7: LED3 On, 8: LED3 Off): </label>"
                "<input type='text' id='option'>"
                "<input type='button' value='Send' onclick='sendOption()'>"
                "</body>"
                "</html>");
  });

  setupRoutes(); // Thiết lập các route của server
  server.begin(); // Khởi động server
}

void handleSendOption() {
  if (server.hasArg("option")) {
    int option = server.arg("option").toInt();
    switch (option) {
      case 1:
        // Bật quạt 
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2, LOW);
        Serial.println("Motor ON");
        server.send(200, "text/plain", "Motor turned ON");
        break;
      case 2:
        // Tắt quạt 
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW);
        Serial.println("Motor OFF");
        server.send(200, "text/plain", "Motor turned OFF");
        break;
      case 5:
        // Bật LED 1 (Bedroom)
        digitalWrite(ledPin1, HIGH);
        Serial.println("LED 1 ON");
        server.send(200, "text/plain", "LED 1 turned ON");
        break;
      case 6:
        // Tắt LED 1
        digitalWrite(ledPin1, LOW);
        Serial.println("LED 1 OFF");
        server.send(200, "text/plain", "LED 1 turned OFF");
        break;
      case 7:
        // Bật LED 2 (DiningRoom)
        digitalWrite(ledPin2, HIGH);
        Serial.println("LED 2 ON");
        server.send(200, "text/plain", "LED 2 turned ON");
        break;
      case 8:
        // Tắt LED 2
        digitalWrite(ledPin2, LOW);
        Serial.println("LED 2 OFF");
        server.send(200, "text/plain", "LED 2 turned OFF");
        break;
      case 9:
        // Bật LED 3 (LivingRoom)
        digitalWrite(ledPin3, HIGH);
        Serial.println("LED 3 ON");
        server.send(200, "text/plain", "LED 3 turned ON");
        break;
      case 10:
        // Tắt LED 3
        digitalWrite(ledPin3, LOW);
        Serial.println("LED 3 OFF");
        server.send(200, "text/plain", "LED 3 turned OFF");
        break;
      default:
        server.send(400, "text/plain", "Invalid option");
    }
  } else {
    server.send(400, "text/plain", "Missing 'option' parameter");
  }
}

void setupRoutes() {
  server.on("/sendOption", handleSendOption); // Route để xử lý tùy chọn điều khiển
}

void loop() {
  server.handleClient(); // Xử lý các yêu cầu từ client
}
