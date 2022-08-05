//修改自：https://blog.csdn.net/wangke0809/article/details/70146378?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522163566195516780261935125%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=163566195516780261935125&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-2-70146378.first_rank_v2_pc_rank_v29&utm_term=esp8266%E4%B8%B2%E5%8F%A3%E9%80%8F%E4%BC%A0%E7%A8%8B%E5%BA%8F&spm=1018.2226.3001.4187
//原作者：wangke0809
#include <ESP8266WiFi.h>
#define MAX_SRV_CLIENTS 3   //最大同时联接数，即你想要接入的设备数量，8266tcpserver只能接入五个，哎
 
const char *ssid = "Baize"; 
const char *password = "baizerobot"; 
 
WiFiServer server(8266);//你要的端口号，随意修改，范围0-65535
WiFiClient serverClients[MAX_SRV_CLIENTS];
 
void setup()
{
    Serial.begin(9600);
    delay(10);
    pinMode(16, OUTPUT);
    digitalWrite(16, 0);
    WiFi.begin(ssid, password);
 
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
    server.begin();
    server.setNoDelay(true);  //加上后才正常些
}
 
void loop()
{
    blink();
 
    uint8_t i;
    if (server.hasClient())
    {
        for (i = 0; i < MAX_SRV_CLIENTS; i++)
        {
            if (!serverClients[i] || !serverClients[i].connected())
            {
                if (serverClients[i]) serverClients[i].stop();//未联接,就释放
                serverClients[i] = server.available();//分配新的
                continue;
            }
 
        }
        WiFiClient serverClient = server.available();
        serverClient.stop();
    }
    for (i = 0; i < MAX_SRV_CLIENTS; i++)
    {
        if (serverClients[i] && serverClients[i].connected())
        {
            digitalWrite(16, 0);//有链接存在,就一直长亮
 
            if (serverClients[i].available())
            {
                while (serverClients[i].available()) 
                    Serial.write(serverClients[i].read());
            }
        }
    }
    if (Serial.available())
    {
        size_t len = Serial.available();
        uint8_t sbuf[len];
        Serial.readBytes(sbuf, len);
        //push UART data to all connected telnet clients
        for (i = 0; i < MAX_SRV_CLIENTS; i++)
        {
            if (serverClients[i] && serverClients[i].connected())
            {
                serverClients[i].write(sbuf, len);  //向所有客户端发送数据
                delay(1);
            }
        }
    }
}
 
 
void blink()
{
    static long previousMillis = 0;
    static int currstate = 0;
 
    if (millis() - previousMillis > 200)  //200ms
    {
        previousMillis = millis();
        currstate = 1 - currstate;
        digitalWrite(16, currstate);
    }
}
