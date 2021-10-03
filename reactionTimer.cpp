#include <EEPROM.h>

// Include the display library code:
#include <LiquidCrystal.h>

// Initialize the display library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Define the pins for the Button & LED
int pinButton = 8;
int pinLED = 9;

// Define variables for the start and end time
int startTime = 0;
int endTime = 0;

void setup()
{
    // Assign the LED & button pins
    pinMode(pinLED, OUTPUT);
    pinMode(pinButton, INPUT);

    analogWrite(6, 34);

    // Set up the LCD's number of columns and rows:
    lcd.begin(16, 2);

    Serial.begin(115200);
}

void loop()
{
    lcd.clear();
    //Display text on screen
    lcd.print("Press To Play");
    //Update display
    //lcd.display();
    //If button to start game is pressed
    if (digitalRead(pinButton) == LOW)
    {
        int result = performTest(5);

        averageTimeScreen(result);

        //String resultToStore = String(result);
        writeToEEPROM(0, result);
        Serial.println(readFromEEPROM(0));

        //Wait 5 seconds on results before restarting
        delay(5000);
    }
    delay(100);
}

int performTest(int testAmount)
{
    int totalTimeSum = 0;

    for (int i = 0; i < testAmount; i++)
    {
        int time = test();
        //Check for cheating by holding the button down
        if (time <= 20)
        {
            cheatScreen();
            i--;
        }
        //Return the result
        else
        {
            totalTimeSum += time;
        }
        lcd.display();
    }
    int totalTime = totalTimeSum / testAmount;
    return totalTime;
}

int test()
{
    readyScreen();
    //Wait a random amount of time between 0.5s and 5s so that users can't guess the start time
    int delayTime = random(500, 5000);
    //Delay the random amount of time
    delay(delayTime);
    //Light up the LED
    digitalWrite(pinLED, HIGH);
    //Record the start time
    startTime = millis();
    //Wait for the user to push the button
    while (digitalRead(pinButton) == HIGH)
    {
        if (((endTime = millis()) - startTime) >= 5000)
        {
            break;
        }
    }
    //Record the button push time
    endTime = millis();
    //Turn off the LED
    digitalWrite(pinLED, LOW);
    //Calculate the total response time
    int time = endTime - startTime;
    lcd.clear();
    return time;
}

void cheatScreen()
{
    //Display cheat screen
    lcd.setCursor(0, 0);
    lcd.println("Don't Hold The");
    lcd.setCursor(0, 2);
    lcd.print("Button Down");
    delay(1000);
    lcd.clear();
}

void readyScreen()
{
    //Display "Ready" countdown with 3 sequential dots
    lcd.setCursor(0, 2);
    lcd.print("Ready");
    lcd.display();
    delay(200);
    lcd.print(".");
    lcd.display();
    delay(200);
    lcd.print(".");
    lcd.display();
    delay(200);
    lcd.print(".");
    lcd.display();
}

void averageTimeScreen(int time)
{
    //Display average time
    lcd.setCursor(0, 0);
    lcd.print("Average Time:");
    lcd.setCursor(0, 2);
    lcd.print(time);
    lcd.print("ms");
}

void writeToEEPROM(int address, int number)
{
    EEPROM.write(address, number >> 8);
    EEPROM.write(address + 1, number & 0xFF);
}

int readFromEEPROM(int address)
{
    byte byte1 = EEPROM.read(address);
    byte byte2 = EEPROM.read(address + 1);
    return (byte1 << 8) + byte2;
}