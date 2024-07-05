#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// Replace these with your network credentials
const char* ssid = "Mi 11x";
const char* password = "incorrect";  // Replace with your Wi-Fi password

// Initialize the ESP8266 Web Server on port 80
ESP8266WebServer server(80);

// Device states
bool lightOn = false;
int thermostatTemp = 20;  // Default temperature
bool securityActivated = false;

// HTML page
const char* htmlPage = R""(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Home Automation Control</title>
</head>
<body>
    <h1>ESP8266 Home Automation</h1>
    <div>
        <h2>Light Control</h2>
        <button onclick="controlDevice('light', 'on')">Turn On</button>
        <button onclick="controlDevice('light', 'off')">Turn Off</button>
    </div>
    <div>
        <h2>Thermostat Control</h2>
        <input type="number" id="thermostat" placeholder="Set temperature" min="10" max="30">
        <button onclick="setTemperature()">Set Temperature</button>
    </div>
    <div>
        <h2>Security System Control</h2>
        <button onclick="controlDevice('security', 'activate')">Activate</button>
        <button onclick="controlDevice('security', 'deactivate')">Deactivate</button>
    </div>
    <script>
        function controlDevice(device, action) {
            var xhr = new XMLHttpRequest();
            xhr.open("POST", "/control", true);
            xhr.setRequestHeader("Content-Type", "application/json");
            xhr.onreadystatechange = function () {
                if (xhr.readyState === 4 && xhr.status === 200) {
                    alert(xhr.responseText);
                }
            };
            var data = JSON.stringify({"device": device, "action": action});
            xhr.send(data);
        }

        function setTemperature() {
            const temp = document.getElementById("thermostat").value;
            controlDevice('thermostat', temp);
        }
    </script>
</body>
</html>
)"";

// Function to handle root ("/") URL
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// Function to handle control requests
void handleControl() {
  if (server.method() == HTTP_POST) {
    // Get the request payload
    String json = server.arg("plain");

    // Log received JSON data
    Serial.println("Received JSON:");
    Serial.println(json);

    // Parse the JSON data
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, json);

    // Check for deserialization errors
    if (error) {
      Serial.println("Failed to parse JSON");
      server.send(400, "application/json", "{\"message\":\"Invalid JSON\"}");
      return;
    }

    // Extract the device and action
    String device = doc["device"];
    String action = doc["action"];

    // Initialize a response message
    String message;

    // Handle device actions
    if (device == "light") {
      if (action == "on") {
        digitalWrite(LED_BUILTIN, LOW);  // Turn on LED (active low)
        lightOn = true;
        message = "Light turned on";
      } else if (action == "off") {
        digitalWrite(LED_BUILTIN, HIGH);  // Turn off LED (active low)
        lightOn = false;
        message = "Light turned off";
      } else {
        message = "Invalid action for light";
      }
    } else if (device == "thermostat") {
      int temp = action.toInt();
      if (temp >= 10 && temp <= 30) {  // Assuming a realistic temperature range
        thermostatTemp = temp;
        message = "Thermostat set to " + String(thermostatTemp) + "°C";
      } else {
        message = "Invalid temperature value";
      }
    } else if (device == "security") {
      if (action == "activate") {
        securityActivated = true;
        message = "Security system activated";
      } else if (action == "deactivate") {
        securityActivated = false;
        message = "Security system deactivated";
      } else {
        message = "Invalid action for security system";
      }
    } else {
      message = "Unknown device";
    }

    // Log the response message
    Serial.println("Response message:");
    Serial.println(message);

    // Send the response back to the client
    server.send(200, "application/json", "{\"message\":\"" + message + "\"}");
  } else {
    server.send(405, "application/json", "{\"message\":\"Method Not Allowed\"}");
  }
}

// Set up the server and Wi-Fi
void setup() {
  Serial.begin(115200);

  // Initialize the onboard LED pin
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // Turn off LED initially (active low)

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set up server routes
  server.on("/", handleRoot);
  server.on("/control", HTTP_POST, handleControl);

  // Start the server
  server.begin();
  Serial.println("HTTP server started at: ");
  Serial.print(WiFi.localIP());
}

// Main loop to handle client requests
void loop() {
  server.handleClient();
}
