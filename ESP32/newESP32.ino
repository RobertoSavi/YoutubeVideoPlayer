#include "WiFi.h"
#include "HTTPClient.h"
// #include <ArduinoJson.h>
// #include <Arduino_JSON.h>
#include <string.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsClient.h>
#include "index.h"

#define RX 16 // goes to 3.3
#define TX 17 // goes to 3.2
#define INTERNET_ACTIVE 1

// const char *ssid = "iPhone di Merak"; // insert here
// const char *password = "carmen123";   // insert here
const char *ssid = "Roberto's Galaxy A52s 5G"; // insert here
const char *password = "aibohphobia";          // insert here
const char *ws_server_host = "16.171.133.32";

const int baud_rate = 115200;

WebSocketsClient webSocket;

String request;
char charRequest[100];
char *token;
const char s[2] = ".";

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.println("WebSocket disconnected");
        break;
    case WStype_CONNECTED:
        Serial.println("WebSocket connected");
        break;
    case WStype_TEXT:
        Serial.printf("Received message: %s\n", payload);
        // Puoi gestire qui il messaggio ricevuto dal server
        break;
    case WStype_BIN:
        Serial.println("Binary message received (not handled)");
        break;
    }
}

int timer = 1;
char c;

void setup()
{
    Serial.begin(baud_rate);
    Serial2.begin(baud_rate, SERIAL_8N1, RX, TX); // define serial channel
    // Disconnect from previous AP
    if (INTERNET_ACTIVE)
    {
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        delay(100);
        scanWiFi(); // checks WiFi APs available
        initWiFi(); // connect to desired AP
    }
    // Initialize WebSocket server
    webSocket.begin(ws_server_host, 5000, "/");
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);

    Serial.println("Setup completed");
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
                    webSocket.sendTXT(0, "volUp");
                }
                else if (strcmp(token, "volDown") == 0)
                {
                    webSocket.sendTXT(0, "volDown");
                }
                else if (strcmp(token, "timePlus") == 0)
                {
                    webSocket.sendTXT(0, "timePlus");
                }
                else if (strcmp(token, "timeMinus") == 0)
                {
                    webSocket.sendTXT(0, "timeMinus");
                }
                else if (strcmp(token, "pause") == 0)
                {
                    webSocket.sendTXT(0, "pause");
                }
                else if (strcmp(token, "play") == 0)
                {
                    webSocket.sendTXT(0, "play");
                }
                else if (strcmp(token, "mute") == 0)
                {
                    webSocket.sendTXT(0, "mute");
                }
                else if (strcmp(token, "unmute") == 0)
                {
                    webSocket.sendTXT(0, "unmute");
                }
                else if (strcmp(token, "prev") == 0)
                {
                    webSocket.sendTXT(0, "prev");
                }
                else if (strcmp(token, "next") == 0)
                {
                    webSocket.sendTXT(0, "next");
                }
                else if (strcmp(token, "lSpee") == 0)
                {
                    webSocket.sendTXT(0, "lSpee");
                }
                else if (strcmp(token, "hSpee") == 0)
                {
                    webSocket.sendTXT(0, "hSpee");
                }
            }
            token = strtok(NULL, s);
        }
    }
}

void sendString(String send)
{
    for (int i = 0; i < send.length(); i++)
    {
        c = send.charAt(i);
        Serial2.write(c);
        timer--;
        if (timer == 0)
        {
            String tmp = Serial2.readStringUntil('%');
            Serial.println(tmp);
            timer = 1;
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
        // Print all APs
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
    Serial.print("\n Connected to: ");
    Serial.println(WiFi.SSID());
}
