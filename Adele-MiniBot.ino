
// OUTPUT PINS

#define LED_PIN 12

#define MOTOR_RIGHT_PWM_FORWARD 10
#define MOTOR_LEFT_PWM_FORWARD 11

#define MOTOR_RIGHT_PWM_BACKWARD 9
#define MOTOR_LEFT_PWM_BACKWARD 6

// INPUT PINS

#define FORWARD_BUTTON_PIN 2
#define BACKWARD_BUTTON_PIN 3
#define RIGHT_BUTTON_PIN 4
#define LEFT_BUTTON_PIN 5

#define START_BUTTON_PIN 7

// CONFIGURATION PARAMETERS

#define ALLOWED_INSTRUCTIONS 10

#define MOTORS_SPEED 255
#define INSTRUCTION_DURATION_MS 1000

void (*instructions[ALLOWED_INSTRUCTIONS])();

uint8_t instructionCounter;

void setup() 
{
  pinMode(FORWARD_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BACKWARD_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);

  pinMode(START_BUTTON_PIN, INPUT_PULLUP);

  pinMode(MOTOR_LEFT_PWM_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_PWM_FORWARD, OUTPUT);

  pinMode(LED_PIN, OUTPUT);

  ledBlink(5);

  //Serial.begin(9600);
}

void loop() 
{ 
	programMode();
	executeProgram();
}

void programMode()
{
	bool startProgram = false;

	instructionCounter = 0;

	int pressedButtons;

	while(instructionCounter < ALLOWED_INSTRUCTIONS && !startProgram)
	{
		pressedButtons = 0;

		void (*readInstruction)();

		if (digitalRead(FORWARD_BUTTON_PIN) == LOW)
		{
			readInstruction = goForward;
			pressedButtons++;
		}

		if (digitalRead(BACKWARD_BUTTON_PIN) == LOW)
		{
			readInstruction = goBackward;
			pressedButtons++;
		}

		if (digitalRead(LEFT_BUTTON_PIN) == LOW)
		{
			readInstruction = rotateLeft;
			pressedButtons++;
		}

		if (digitalRead(RIGHT_BUTTON_PIN) == LOW)
		{
			readInstruction = rotateRight;
			pressedButtons++;
		}

		if (pressedButtons == 1)
		{
			instructions[instructionCounter++] = readInstruction;
			ledBlink(1);
		}

		/*Serial.print("IC: ");
		Serial.print(instructionCounter);
		Serial.print("\n");

		Serial.print("PB: ");
		Serial.print(pressedButtons);
		Serial.print("\n");*/

		startProgram = digitalRead(START_BUTTON_PIN) == LOW;
	}
}

void executeProgram()
{
	ledBlink(3);

	for (int currentInstruction = 0; 
		currentInstruction < instructionCounter; 
		++currentInstruction)
	{
		instructions[currentInstruction]();
	}

	ledBlink(3);
}

void ledBlink(uint8_t times)
{
	for (int i = 0; i < times; ++i)
	{
		delay(500);
		digitalWrite(LED_PIN, HIGH);
		delay(500);
		digitalWrite(LED_PIN, LOW);
	}
}

void goForward()
{
	analogWrite(MOTOR_RIGHT_PWM_FORWARD, MOTORS_SPEED);
	analogWrite(MOTOR_LEFT_PWM_FORWARD, MOTORS_SPEED - 20);
	delay(INSTRUCTION_DURATION_MS);
	analogWrite(MOTOR_RIGHT_PWM_FORWARD, 0);
	analogWrite(MOTOR_LEFT_PWM_FORWARD, 0);
}

void goBackward()
{
	analogWrite(MOTOR_RIGHT_PWM_BACKWARD, MOTORS_SPEED);
	analogWrite(MOTOR_LEFT_PWM_BACKWARD, MOTORS_SPEED - 20);
	delay(INSTRUCTION_DURATION_MS);
	analogWrite(MOTOR_RIGHT_PWM_BACKWARD, 0);
	analogWrite(MOTOR_LEFT_PWM_BACKWARD, 0);
}

void rotateLeft()
{
	analogWrite(MOTOR_RIGHT_PWM_FORWARD, MOTORS_SPEED);
	analogWrite(MOTOR_LEFT_PWM_BACKWARD, MOTORS_SPEED);
	delay(INSTRUCTION_DURATION_MS / 4 * 3);
	analogWrite(MOTOR_RIGHT_PWM_FORWARD, 0);
	analogWrite(MOTOR_LEFT_PWM_BACKWARD, 0);
}

void rotateRight()
{
	analogWrite(MOTOR_LEFT_PWM_FORWARD, MOTORS_SPEED);
	analogWrite(MOTOR_RIGHT_PWM_BACKWARD, MOTORS_SPEED);
	delay(INSTRUCTION_DURATION_MS / 4 * 3);
	analogWrite(MOTOR_LEFT_PWM_FORWARD, 0);
	analogWrite(MOTOR_RIGHT_PWM_BACKWARD, 0);
}