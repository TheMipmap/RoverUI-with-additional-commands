#include <Wire.h>
#include <Zumo32U4.h>


// instanciate the different Zumo classes
Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4LCD lcd;
Zumo32U4Encoders encoders;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;

int accCountsR = 0;
int commandSelected = 0; // 0 -> F, 1 -> backwards, 2 -> right, 3 -> left
int speedSelected = 100; // A speed between 0 and 400
int timeSelected = 1; // Seconds the command should run
int actualStage = 0; // The current stage of the Rover
int nCommands = 1; //Number of commands
int iterations = 0; //Iterations of stage 1,2 and 3

void readEncoder() {
  accCountsR += encoders.getCountsAndResetRight(); //accCountsR = accCountsR + encoders.getCountsAndResetRight();
}

void beep() {
  buzzer.playNote(NOTE_A(4), 20, 15);
}

void LCD_stage0(int numberOfCommands) {
  
  String amountOfCommands = String(numberOfCommands);

  
  if (numberOfCommands > 15 || numberOfCommands < 1) amountOfCommands = "Error";
  
  
  lcd.clear();
  lcd.print("Commands");
  lcd.gotoXY(0,1);
  lcd.print(amountOfCommands);
}

void stage0() {
  
  readEncoder(); //Read encoder to change number of commands
  
  if (accCountsR > 100) {
          beep();
          nCommands += 1;
          accCountsR -= 100;
          if (commandSelected > 15) commandSelected = 1;
       } else if (accCountsR < -100) {
        beep();
        nCommands -= 1;
        accCountsR += 100;
          if (nCommands < 1) nCommands = 15;
       }
  // Show number of commands on display
  LCD_stage0(nCommands);

  //Move on to the next stage, if a button is pressed
  if (buttonA.isPressed()) {
          actualStage = 1;
          buttonA.waitForRelease();
        }
}

void LCD_stage1(int command) {
  String nameCommand;
  switch (command) {
    case 0:
    nameCommand = "Forward";
    break;
    
    case 1:
    nameCommand = "Backward";
    break;
    
    case 2:
    nameCommand = "Right";
    break;
 
    case 3:
    nameCommand = "Left";
    break;

    default:
    nameCommand = "Error";
  }
  
  lcd.clear();
  lcd.print("Command>");
  lcd.gotoXY(0,1);
  lcd.print(nameCommand);
}

void stage1() {
      // Stage 1 --> select command
        
         // read the encoders (function)
        readEncoder();
        
         // if it's larger or lower than threshold then
            // increment or decrement the selected command
       if (accCountsR > 100) {
          beep();
          commandSelected += 1;
          accCountsR -= 100;
          if (commandSelected > 3) commandSelected = 0;
       } else if (accCountsR < -100) {
        beep();
        commandSelected -= 1;
        accCountsR += 100;
          if (commandSelected < 0) commandSelected = 3;
       }

       Serial.println("Counts: " + String(accCountsR) + "\n" + "Command: " + String(commandSelected));
          
        //Show it on the screen
            //Title
            //In which command we are 
        LCD_stage1(commandSelected);

        
        //Until button pressed?
            //if yes I jump to the next stage and store the command
        if (buttonA.isPressed()) {
          actualStage = 2;
          buttonA.waitForRelease();
        }
}

void LCD_stage2(int speed) {
  String nameSpeed;
  switch (speed) {
    case 50:
    nameSpeed = "50";
    break;
    
    case 100:
    nameSpeed = "100";
    break;
    
    case 150:
    nameSpeed = "150";
    break;
 
    case 200:
    nameSpeed = "200";
    break;

    case 250:
    nameSpeed = "250";
    break;

    case 300:
    nameSpeed = "300";
    break;

    case 350:
    nameSpeed = "350";
    break;

    case 400:
    nameSpeed = "400";
    break;

    default:
    nameSpeed = "Error";
  }
  
  lcd.clear();
  lcd.print("Speed >");
  lcd.gotoXY(0,1);
  lcd.print(nameSpeed);
}

void stage2() {
          // read the encoders (same function)
       readEncoder();
       
        // if it's larger or lower than threshold then
            // increment or decrement the selected speed  
         if (accCountsR > 100) {
           speedSelected += 50;
           beep();
           accCountsR -= 100;
           if (speedSelected > 400) speedSelected = 50;
         } else if (accCountsR < -100) {
            speedSelected -= 50;
            beep();
            accCountsR += 100;
            if (speedSelected < 50) speedSelected = 400;
         }

        //Show the Speed on the screen
        LCD_stage2(speedSelected);

        //Until button pressed?
            //if yes I jump to the next stage and store the command
        if (buttonA.isPressed()) {
          actualStage = 3;
          buttonA.waitForRelease();
        }
        
}

void LCD_stage3(int time) {
  
  String nameTime = String(time);

  
  if (time > 15 || time < 1) nameTime = "Error";
  

  
  lcd.clear();
  lcd.print("Time >");
  lcd.gotoXY(0,1);
  lcd.print(nameTime);
}

void stage3() {
   //Read encoders
   readEncoder();
   
   //if it is larger or lower than threshold do 
      //increment or decrement the selected time
    if (accCountsR > 100) {
        timeSelected += 1;
        beep();
        accCountsR -= 100;
        if (timeSelected > 15) timeSelected = 1;
    } else if (accCountsR < -100) {
        timeSelected -= 1;
        beep();
        accCountsR += 100;
        if (timeSelected < 1) timeSelected = 1;
    }
    
   //Show the time  on the display
    LCD_stage3(timeSelected);

   //Until button is pressed, wait further
    if (buttonA.isPressed()) {
      actualStage = 4;
      buttonA.waitForRelease();
    }
}

void stage4() {
  // run the command
  for (int i = 0; i < nCommands; i++) {

  switch (commandSelected) {
    case 0:
    Serial.println("Case 0");
    motors.setSpeeds(speedSelected,speedSelected);
    delay(timeSelected * 1000);
    motors.setSpeeds(0,0);
    break;
    
    case 1:
    Serial.println("Case 1");
    motors.setSpeeds(-speedSelected,-speedSelected);
    delay(timeSelected * 1000);
    motors.setSpeeds(0,0);
    break;

    case 2:
    Serial.println("Case 2");
    motors.setSpeeds(-speedSelected,speedSelected);
    delay(timeSelected * 1000);
    motors.setSpeeds(0,0);
    break;

    case 3:
    Serial.println("Case 3");
    motors.setSpeeds(speedSelected,-speedSelected);
    delay(timeSelected * 1000);
    motors.setSpeeds(0,0);
    break;

    default:
    Serial.println("Something went wrong in stage 3");
    lcd.clear();
    lcd.print("SomeTINK");
    lcd.gotoXY(0,1);
    lcd.print("WRONG!!!");
  }
  }

  // Reset the encoder
  accCountsR = encoders.getCountsAndResetRight();
  accCountsR = 0;

  //Add a delay before next command
  delay(300);
  
  //Set the actualStage to 0, to reset the program
  actualStage = 0;
  
}



void setup() {
  Serial.begin(9600);
}


void loop() {
  
      // stage0 --> select number of commands
        if (actualStage == 0) {
          stage0(); // Catch the const for the array
          delay(50);
        }

      // Stage1 -> Select command
        if (actualStage == 1) {
        stage1();
        delay(50);
        }

    // stage 2 --> Select speeed 
        if (actualStage == 2) {
          stage2();
          delay(50);
        }

    // stage 3 --> Select time
        if (actualStage == 3) {
          stage3();
          delay(50);
        }
    
    // Stage 4 --> Run the program
        if(actualStage == 4) {
          Serial.println("Im in stage 3!");
          stage4();
          delay(50);
        }

  
}





