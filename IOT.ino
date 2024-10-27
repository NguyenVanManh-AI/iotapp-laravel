#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h> // Thêm thư viện Servo

// Khai báo các chân điều khiển động cơ và LED
const int motorPin1 = D6; // Chân IN1 của mạch điều khiển động cơ
const int motorPin2 = D5; // Chân IN2 của mạch điều khiển động cơ
const int ledPin1 = D1;   // Chân kết nối LED 1
const int ledPin2 = D2;   // Chân kết nối LED 2
const int ledPin3 = D3;   // Chân kết nối LED 3

Servo myservo; // Khai báo đối tượng Servo 
// Servo dây cam nối với D4 của wifi , dây đỏ nối 3V3 của wifi, dây đà nối GND của wifi
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

  // Thiết lập Servo
  myservo.attach(D4); // Gán chân D4 cho Servo

  // Thiết lập server
  server.on("/", [] {
    server.send(200, "text/html",
                "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<title>Control DC Motor, Servo, and LED</title>"
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
                "<label>Option (1: Fan On, 2: Fan Off, 3: Open Door, 4: Close Door, 5: Bedroom On, 6: Bedroom Off, 7: DiningRoom On, 8: DiningRoom Off, 9: LivingRoom On, 10: LivingRoom Off): </label>"
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
      case 3:
        // Quay Servo 90 độ (Mở cửa)
        myservo.write(90);
        Serial.println("Servo 90 degrees");
        server.send(200, "text/plain", "Servo turned to 90 degrees");
        break;
      case 4:
        // Quay Servo về 0 độ (Đóng cửa)
        myservo.write(0);
        Serial.println("Servo 0 degrees");
        server.send(200, "text/plain", "Servo turned to 0 degrees");
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
