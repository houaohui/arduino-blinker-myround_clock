#define BLINKER_WIFI
#include <FastLED.h>
#include <Blinker.h>

#define NUM_LEDS 60
#define LED_PIN  7
CRGB leds[NUM_LEDS];

uint8_t background_colorR = 0 , background_colorG = 0, background_colorB = 0;
uint8_t lamp_colorR = 255 , lamp_colorG = 255 , lamp_colorB = 255;
uint8_t sec_colorR = 255 , sec_colorG = 0 , sec_colorB = 0;
uint8_t min_colorR = 0 , min_colorG = 255 , min_colorB = 0;
uint8_t hou_colorR = 0 , hou_colorG = 0 , hou_colorB = 255;
char auth[] = "cd59078fea1e";
char ssid[] = "16s";
char pswd[] = "12345678";

uint8_t apps = 0;


uint8_t lampmode = 0, colormode = 0;
uint8_t sec = 0, minu = 0, hou = 0;
int8_t set_hou, set_minu, set_sec;

uint8_t refreash = 0;
// 新建组件对象
BlinkerButton Button1("btn-abc");
BlinkerRGB RGB1("RGB_1");

////////////////////组件////////////////////////
void button1_callback(const String & state)
{
  if (state == BLINKER_CMD_BUTTON_TAP)  //在不同模式下选择功能
  {
    if (apps == 0)
    {
      refreash = 1;
      Button1.text("时钟模式");
      Button1.print();
    }
    else if (apps == 1)
    {
      lampmode++;
      if (lampmode == 3)
        lampmode = 0;
      switch (lampmode)
      {
        case 0: Button1.text("台灯模式1"); Button1.print(); break;
        case 1: Button1.text("台灯模式2"); Button1.print(); break;
        case 2: Button1.text("台灯模式3"); Button1.print(); break;
      }
    }
    else if (apps == 2)
    {
      colormode++;
      if (colormode == 3)
        colormode = 0;

      switch (colormode)
      {
        case 0: Button1.text("设置秒颜色"); Button1.print(); break;
        case 1: Button1.text("设置分颜色"); Button1.print(); break;
        case 2: Button1.text("设置时颜色"); Button1.print(); break;
      }
    }
  }
  else if (state == BLINKER_CMD_BUTTON_PRESSED ) //切换模式
  {
    lampmode = 0;  //清零
    colormode = 0;  //
    apps++;
    if (apps == 3)
      apps = 0;
    switch (apps)
    {
      case 0: Button1.text("时钟模式"); Button1.print(); break;
      case 1: Button1.text("台灯模式1");  Button1.print(); break;
      case 2: Button1.text("设置秒颜色"); Button1.print(); break;
    }
  }
}

void rgb1_callback(uint8_t r_value, uint8_t g_value, uint8_t b_value, uint8_t bright_value)
{
  BLINKER_LOG("R value: ", r_value);
  BLINKER_LOG("G value: ", g_value);
  BLINKER_LOG("B value: ", b_value);
  BLINKER_LOG("Rrightness value: ", bright_value);
  if (apps == 0)
  {
    background_colorR = map(r_value, 0, 255, 0, bright_value);
    background_colorG = map(g_value, 0, 255, 0, bright_value);
    background_colorB = map(b_value, 0, 255, 0, bright_value);
  }
  else if (apps == 1)
  {
    lamp_colorR = map(r_value, 0, 255, 0, bright_value);
    lamp_colorG = map(g_value, 0, 255, 0, bright_value);
    lamp_colorB = map(b_value, 0, 255, 0, bright_value);
  }
  else if (apps == 2)
  {
    if (colormode == 0)
    {
      sec_colorR = map(r_value, 0, 255, 0, bright_value);
      sec_colorG = map(g_value, 0, 255, 0, bright_value);
      sec_colorB = map(b_value, 0, 255, 0, bright_value);
    }
    else if (colormode == 1)
    {
      min_colorR = map(r_value, 0, 255, 0, bright_value);
      min_colorG = map(g_value, 0, 255, 0, bright_value);
      min_colorB = map(b_value, 0, 255, 0, bright_value);
    }
    else if (colormode == 2)
    {
      hou_colorR = map(r_value, 0, 255, 0, bright_value);
      hou_colorG = map(g_value, 0, 255, 0, bright_value);
      hou_colorB = map(b_value, 0, 255, 0, bright_value);
    }
  }
}


//////////////////////////////////////////////////

//时钟模式
void clock_display()
{
  uint8_t i;
  if (refreash)
  {
    for (i = 0; i < 60; i++)
    {
      delay(20);
      leds[i] = CRGB(255, 255, 255);
      FastLED.show();
    }

    set_hou = Blinker.hour() - 12;
    set_minu = Blinker.minute();
    set_sec = Blinker.second();
    BLINKER_LOG("set_hou: ", set_hou);
    BLINKER_LOG("set_minu: ", set_minu);
    BLINKER_LOG("set_sec: ", set_sec);
    sec = 0;
    minu = 0;
    hou = 0;

    for (i = 0; i < 60; i++)
    {
      delay(20);
      leds[i] = CRGB(0, 0, 0);
      FastLED.show();
    }

    while ((hou != set_hou) || (minu != set_minu))
    {
      i = 0;
      while (hou * 5 != set_hou * 5)
      {
        i++;
        if (i % 5 == 0)
        {
          hou++;
        }
        Blinker.run();  //防止心跳断开
        leds[i - 1] = CRGB(0, 0, 0);
        leds[i] = CRGB(hou_colorR, hou_colorG, hou_colorB);
        FastLED.show();
      }

      while (minu != set_minu)
      {
        Blinker.run();  //防止心跳断开

        minu++;
        if ((minu - 1) != hou * 5)
        {
          leds[minu - 1] = CRGB(0, 0, 0);
        }
        if (minu != hou * 5)
        {
          leds[minu] = CRGB(min_colorR, min_colorG, min_colorB);
        }
        FastLED.show();
      }

      while (sec != set_sec)
      {
        Blinker.run();  //防止心跳断开
        sec++;

        if (((sec - 1) != hou * 5) && ((sec - 1) != minu))
        {
          leds[sec - 1] = CRGB(0, 0, 0);
        }
        if ((sec != hou * 5) && (sec != minu))
        {
          leds[sec] = CRGB(sec_colorR, sec_colorG, sec_colorB);
        }
        FastLED.show();
      }

    }

    refreash = 0;
  }
  else
  {
    delay(950);
    sec++;
    if (sec == 60)
    {
      sec = 0;
      minu++;
      if (minu == 60)
      {
        minu = 0;
        hou++;
        if (hou == 12)
        {
          hou = 0;
        }
      }
    }
    for (i = 0; i < 60; i++)
    {
      if (i == sec || i == minu || i == hou * 5)
      {
        leds[sec] = CRGB(sec_colorR, sec_colorG, sec_colorB);   //red
        leds[minu] = CRGB(min_colorR, min_colorG, min_colorB);   //green
        leds[hou * 5] = CRGB(hou_colorR, hou_colorG, hou_colorB); //blue
      }
      else
      {
        leds[i] = CRGB(background_colorR, background_colorG, background_colorB); //
      }
    }
    FastLED.show();
  }
}

///////////////////////////////
//lamp模式
void lamp_display()
{
  uint8_t i = 0;
  switch (lampmode)
  {
    case 0: delay(20);
      for (i = 0; i < 60; i++)
      {
        leds[i] = CRGB(lamp_colorR, lamp_colorG, lamp_colorB);
      }
      FastLED.show(); break;
    case 1:
      for (i = 0; i < 60; i++)
      {
        leds[i] = CRGB(i * 4, 255 - i * 4, i * 4);
        FastLED.show();
      }
      break;
    case 2: 
      sec++;
      if (sec == 60)
      {
        sec = 0;
        minu++;
        if (minu == 60)
        {
          minu = 0;
          hou++;
          if (hou == 12)
          {
            hou = 0;
          }
        }
      }
      for (i = 0; i < 60; i++)
      {
        if (i == sec || i == minu || i == hou * 5)
        {
          leds[sec] = CRGB(sec_colorR, sec_colorG, sec_colorB);   //red
          leds[minu] = CRGB(min_colorR, min_colorG, min_colorB);   //green
          leds[hou * 5] = CRGB(hou_colorR, hou_colorG, hou_colorB); //blue
        }
        else
        {
          leds[i] = CRGB(background_colorR, background_colorG, background_colorB); //
        }
      }
      FastLED.show(); break;
  }
}


//////////////////////////////
//设置指针颜色
void set_color()
{
  uint8_t i = 0;
  switch (colormode)
  {
    case 0: delay(20);
      for (i = 0; i < 60; i++)
      {
        leds[i] = CRGB(sec_colorR, sec_colorG, sec_colorB);
      }
      FastLED.show(); break;
    case 1: delay(20);
      for (i = 0; i < 60; i++)
      {
        leds[i] = CRGB(min_colorR, min_colorG, min_colorB);
      }
      FastLED.show(); break;
    case 2: delay(20);
      for (i = 0; i < 60; i++)
      {
        leds[i] = CRGB(hou_colorR, hou_colorG, hou_colorB);
      }
      FastLED.show(); break;
  }
}

void setup()
{
  // 初始化串口
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);//串口输出
  // 初始化有LED的IO
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  ///////////////////////////
  // 初始化blinker
  Blinker.begin(auth, ssid, pswd);
  //Blinker.attachData(dataRead);
  Button1.attach(button1_callback);
  RGB1.attach(rgb1_callback);
  Blinker.setTimezone(8.0);
}



void loop() {
  Blinker.run();
  switch (apps)
  {
    case 0: clock_display(); break;
    case 1: lamp_display(); break;
    case 2: set_color(); break;
  }

}
