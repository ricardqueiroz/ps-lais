#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define button 7
#define sensor A3
int v = 0; 
int i = 0; //incrementer
long v2 = 0;
long v3 = 0;
long media = 0;

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  pinMode (button, INPUT);
  Serial.begin(9600);
  lcd.init();// initialize the lcd 
  lcd.init();
  lcd.backlight();
}

void loop()
{
  v = 0; 
  i = 0; 
  v2 = 0;
  v3 = 0;
  media = 0;
  
  while (digitalRead(button) == LOW){
    lcd.setCursor(0,0);
    lcd.print ("Insira o sensor");
    lcd.setCursor(0,1);
    lcd.print ("e aperte o botao");
    Serial.println ("Insira o sensor e aperte o botão");
  }
  //collects 10000 measurements from the water sample and averages them
  while (i<10000){  
    v = analogRead (sensor);
    //only performs operations if the values ​​are within the standard
    if (v>0 && v<1022){
      //the first 10 measurements will be made regardless of the values ​​obtained
      if (i<100){
        v2 = v2+v;
        i++; 
      }
      //from the 11th measurement onwards the code performs a data filtering to prevent the variance from being greater than 20
      else{
        //for this, it makes an average with the values ​​obtained so far and uses it as a comparison parameter
        media = v2/(i+1);
        if(v > (media-20) && v < (media+20)){
          v2 = v2+v;
          i++; 
        }
      }
    }
    //if the values ​​are out of the standard, it warns the user that the measurement is going wrong
    else{
      lcd.setCursor(0,0);
      lcd.print ("A posi. da ponta");
      lcd.setCursor(0,1);
      lcd.print ("esta incorreta");
      Serial.println ("A posição da ponteira está incorreta");
    }
  }   
  //calculates the average of all values ​​obtained
  v3 = v2/(i+1);
  //uses tabulated parameters, obtained previously, to recognize whether the sample contains sugar or contains salt
  if (v3 > 500){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print ("A amostra");
    lcd.setCursor(0,1);
    lcd.print ("contem acucar");
    Serial.println ("A amostra contem açucar");
    delay (5000);
  }
  else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print ("A amostra");
    lcd.setCursor(0,1);
    lcd.print ("contem sal");
    Serial.println ("A amostra contem sal");
    delay (5000);
  }
}
