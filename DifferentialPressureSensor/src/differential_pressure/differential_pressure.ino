#include <SparkFun_SDP3x_Arduino_Library.h>
#include <Wire.h>

SDP3X mySensor;

const float pitotCoefficient = 1.0; // ピトー係数を設定
const float airDensity = 1.225; // 空気の密度 (kg/m^3) を設定（1.225 kg/m^3）

void setup() {
  Serial.begin(115200);
  while (!Serial); // シリアル接続が確立されるまで待機
  Serial.println(F("SDP3X Example"));

  Wire.begin();

  // 連続測定モードを停止して初期化
  mySensor.stopContinuousMeasurement();

  // センサーを初期化
  if (mySensor.begin(0x22) == false) { // ここでアドレスを指定
    Serial.println(F("SDP3X not detected. Check connections. Freezing..."));
    while (1);
  }

  // 質量流量温度補償と平均化を有効にして連続測定モードを開始
  mySensor.startContinuousMeasurement(true, true);
}

void loop() {
  float diffPressure;
  float temperature;

  // センサーから平均化された差圧と温度を読み取る
  mySensor.readMeasurement(&diffPressure, &temperature); // 測定値を読み取る

  // ピトー管の速度計算
  float velocity = pitotCoefficient * sqrt((2 * diffPressure) / airDensity);

  // 測定結果をシリアルモニタに出力
  Serial.print(F("Differential pressure is: "));
  Serial.print(diffPressure, 2);
  Serial.print(F(" (Pa)  Temperature is: "));
  Serial.print(temperature, 2);
  Serial.print(F(" (C)  Velocity is: "));
  Serial.print(velocity, 2);
  Serial.println(F(" (m/s)"));

  // アラート条件のチェック
  if (velocity >= 6.0 && velocity <= 8.0) {
    Serial.println(F("Alert: Velocity is in the range of 6-8 m/s!"));
  } else if (velocity == 18.0) {
    Serial.println(F("Alert: Velocity is exactly 18 m/s!"));
  }

  delay(500);
}
