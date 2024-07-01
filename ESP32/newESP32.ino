#include "WiFi.h"
#include "HTTPClient.h"
#include <WebSocketsClient.h>
#include <string.h>

#define RX 16 // goes to 3.3
#define TX 17 // goes to 3.2
#define INTERNET_ACTIVE 1

const char *ssid = "Roberto's Galaxy A52s 5G"; // insert here
const char *password = "aibohphobia";          // insert here
const int baud_rate = 115200;

WebSocketsClient webSocket; // Create a WebSocket client object

String request;
char charRequest[100];
char *token;
const char s[2] = ".";

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.println("WebSocket Disconnected");
        break;
    case WStype_CONNECTED:
        Serial.println("WebSocket Connected");
        break;
    case WStype_TEXT:
        Serial.printf("Received text: %s\n", payload);
        break;
    }
}

int timer = 1;
char c;

void setup()
{
    Serial.begin(baud_rate);
    Serial2.begin(baud_rate, SERIAL_8N1, RX, TX); // define serial channel

    if (INTERNET_ACTIVE)
    {
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        delay(100);
        scanWiFi(); // checks WiFi APs available
        initWiFi(); // connect to desired AP
    }

    // Initialize WebSocket client and set event handler
    webSocket.begin("16.171.133.32", 5000, "/");
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000); // Try to reconnect every 5s if disconnected

    Serial.print("ESP32 connected to WebSocket server with IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Setup completed");
    s
}

void loop()
{
    webSocket.loop();

    while (Serial2.available())
    {
        int length = Serial2.readBytesUntil('\0', charRequest, 100);
        Serial.println(charRequest);
        token = strtok(charRequest, s);
        while (token != NULL)
        {
            Serial.println(token);
            if (strcmp(token, "") != 0)
            {
                if (strcmp(token, "volUp") == 0)
                {
                    webSocket.sendTXT("volUp");
                }
                else if (strcmp(token, "volDown") == 0)
                {
                    webSocket.sendTXT("volDown");
                }
                else if (strcmp(token, "timePlus") == 0)
                {
                    webSocket.sendTXT("timePlus");
                }
                else if (strcmp(token, "timeMinus") == 0)
                {
                    webSocket.sendTXT("timeMinus");
                }
                else if (strcmp(token, "pause") == 0)
                {
                    webSocket.sendTXT("pause");
                }
                else if (strcmp(token, "play") == 0)
                {
                    webSocket.sendTXT("play");
                }
                else if (strcmp(token, "mute") == 0)
                {
                    webSocket.sendTXT("mute");
                }
                else if (strcmp(token, "unmute") == 0)
                {
                    webSocket.sendTXT("unmute");
                }
                else if (strcmp(token, "prev") == 0)
                {
                    webSocket.sendTXT("prev");
                }
                else if (strcmp(token, "next") == 0)
                {
                    webSocket.sendTXT("next");
                }
                else if (strcmp(token, "lSpee") == 0)
                {
                    webSocket.sendTXT("lSpee");
                }
                else if (strcmp(token, "hSpee") == 0)
                {
                    webSocket.sendTXT("hSpee");
                }
            }
            token = strtok(NULL, s);
        }
    }
}

void scanWiFi()
{
    Serial.println("Scanning...");
    int n = WiFi.scanNetworks();
    if (n == 0)
    {
        Serial.println("No networks found");
    }
    else
    {
        Serial.println("Network found");
        for (int i = 0; i < n; ++i)
        {
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " unencrypted" : " encrypted");
            delay(10);
        }
    }
    Serial.println("");
}

void initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }
    Serial.print("\nConnected to: ");
    Serial.println(WiFi.SSID());
}
