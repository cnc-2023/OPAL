#include "main.h"

#include "SPIRenderer.h"

// int kppsTime = 1000000 / (20 * 1000);
int kppsTime = 1000 * 1000;

volatile unsigned long timeOld;

SPIRenderer *renderer;

void setup()
{
  Serial.begin(115200);

  renderer = new SPIRenderer();
  renderer->Start();

  Serial.print("RAM free:");
  Serial.println(ESP.getFreeHeap());

  // kppsTime = kppsTime * 1000;
}

int c = 0;
void loop()
{
  // 如果串口可用，则从串口读取坐标数据，设置renderer的坐标，坐标数据格式为 x,y,如 11.11,22.22 ,每行一个坐标
  if (Serial.available())
  {
    String str = Serial.readStringUntil('\n');
    Serial.println(str);
    int index = str.indexOf(',');
    if (index > 0)
    {
      String x = str.substring(0, index);
      String y = str.substring(index + 1);
      renderer->SetPosition(x.toFloat(), y.toFloat());

      Serial.print("x:");
      Serial.print(x);
      Serial.print(" y:");
      Serial.println(y);
    }
  }
  // if (micros() - timeOld >= kppsTime)
  // {
  //   timeOld = micros();
  //   // draw_task();
  //   Serial.println(c++);
  //   c = c % 10000;
  // }
}
