#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

// ---------- CONFIG ----------
const char* ssid = "Wokwi-GUEST";
const char* password = "";                // Wokwi simula sem senha
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* topic = "equipe/sensor";
int caesarShift = 3;                      // chave da cifra de César

// I2C LCD (endereço 0x27, 16 colunas x 2 linhas)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ----------------------------
WiFiClient espClient;
PubSubClient client(espClient);

// Função César (aplica shift em caracteres imprimíveis 32..126)
String caesarEncrypt(const String &plain, int shift) {
  String out;
  for (size_t i = 0; i < plain.length(); ++i) {
    unsigned char c = plain[i];
    if (c >= 32 && c <= 126) {
      int base = 32;
      int range = 95; // 126 - 32 + 1
      char cc = (char)(base + ((c - base + shift) % range + range) % range);
      out += cc;
    } else {
      out += c;
    }
  }
  return out;
}

void connectWiFi(){
  Serial.print("Conectando WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 40) {
    delay(250);
    Serial.print(".");
    tries++;
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi OK, IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Falha WiFi (simulação pode ainda funcionar).");
  }
}

void reconnectMQTT(){
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    if (client.connect("ESP32Sender_LCD")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 2s");
      delay(2000);
    }
  }
}

// Mostra mensagem cifrada no LCD com cabeçalho fixo.
// Se o texto for maior que 16 chars, faz rolagem horizontal simples.
void displayCiphered(const String &cipher) {
  lcd.clear();
  // Linha 0: cabeçalho (curto)
  lcd.setCursor(0,0);
  lcd.print("Criptografia");
  // Linha 1: texto introdutorio ou começo da mensagem
  delay(600);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Criptografia ef.");
  lcd.setCursor(0,1);
  lcd.print("Essa e sua msg:");
  delay(1000);

  // Agora exibe a mensagem cifrada com rolagem
  int width = 16;
  String text = cipher;
  if ((int)text.length() <= width) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Criptografia ok");
    lcd.setCursor(0,1);
    lcd.print(text);
    return;
  }
  // rolagem: mostra janelas de 16 chars
  for (int start = 0; start <= (int)text.length() - width; ++start) {
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("Criptografia ok");
    lcd.setCursor(0,1);
    lcd.print(text.substring(start, start + width));
    delay(350);
    // permite que o loop principal seja responsivo (não obrigatório)
    if (WiFi.status() != WL_CONNECTED) {
      // tenta reconectar wifi passivamente
    }
  }
  // depois mostra o final por 1s
  delay(800);
}

void setup() {
  Serial.begin(115200);
  delay(200);

  // inicializa LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Iniciando...");
  delay(600);

  connectWiFi();
  client.setServer(mqtt_server, mqtt_port);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MQTT Sender");
  lcd.setCursor(0,1);
  lcd.print("Pronto");
  delay(600);
}

unsigned long lastSend = 0;
void loop() {
  if (!client.connected()) reconnectMQTT();
  client.loop();

  unsigned long now = millis();
  if (now - lastSend >= 3000) { // envia a cada 3s
    lastSend = now;

    // Simula leitura de sensor (temperatura)
    float simulatedTemp = 25.0 + 4.0 * sin((float)now / 6000.0) + (random(-50,50)/100.0);
    String plain = String("{\"device_id\":\"dev1\",\"sensor\":\"temp\",\"value\":") + String(simulatedTemp, 2) + String(",\"ts\":") + String(now) + String("}");
    String cipher = caesarEncrypt(plain, caesarShift);

    // Publica no MQTT
    bool ok = client.publish(topic, cipher.c_str());
    Serial.print("Enviado (plain): ");
    Serial.println(plain);
    Serial.print("Enviado (cipher): ");
    Serial.println(cipher);
    Serial.print("MQTT publish : ");
    Serial.println(ok ? "OK" : "FAIL");

    // Mostra no LCD a informação pedida
    // Primeiro linha fixa com sua frase pedida, segundo mostra a mensagem (ou rola)
    // Para ser mais fiel ao pedido:
    lcd.clear();
    lcd.setCursor(0,0);
    // mostra exatamente a frase pedida (encaixada em 16 col)
    lcd.print("Criptografia");
    // pequena pausa e depois a linha completa com a mensagem (com rolagem)
    delay(600);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Criptografia ef.");
    lcd.setCursor(0,1);
    lcd.print("Essa e sua msg:");
    delay(800);

    displayCiphered(cipher);
  }

  // evita travar
  delay(20);
}

