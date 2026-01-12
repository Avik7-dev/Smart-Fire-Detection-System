#define TEMP_PIN A0
#define BUZZER_PIN 9
#define LED_PIN 8

float lastTemp = 0;
unsigned long lastTime = 0;

const float SAFE_TEMP = 100.0;        // Absolute temperature limit (°C)
const float SAFE_RISE = 10.0;         // Rate of rise (°C/sec)
void setup()
{
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);

  lastTime = millis();
}

void loop()
{
  int sensorValue = analogRead(TEMP_PIN);

  // Convert LM35 reading to temperature
  float voltage = sensorValue * (5.0 / 1023.0);
  float temperature = voltage * 100.0;  // LM35: 10mV/°C

  unsigned long currentTime = millis();
  float timeDiff = (currentTime - lastTime) / 1000.0;

  float rateOfRise = 0;

  if (timeDiff > 0) {
    rateOfRise = (temperature - lastTemp) / timeDiff;
  }

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" °C | Rise Rate: ");
  Serial.print(rateOfRise);
  Serial.println(" °C/sec");

  // Fire detection logic
  if (temperature > SAFE_TEMP || rateOfRise > SAFE_RISE) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }

  lastTemp = temperature;
  lastTime = currentTime;

  delay(1000);
}