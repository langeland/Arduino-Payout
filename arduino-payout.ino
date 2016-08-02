#include <Servo.h>


Servo servo;

const int servoPin = D1;

const int servoMax = 55;
const int servoMin = 145;
const int servoDalay = 10;
int serverPos = 0;    // variable to store the servo position
int payoutAmount;
int payoutCounter = 0;

int payout(int amount){
	Serial.print("    Payout ");

	for (payoutCounter = 1; payoutCounter <= amount; payoutCounter += 1) {
		for (serverPos = servoMax; serverPos <= servoMin; serverPos += 1) {
			servo.write(serverPos);
			delay(servoDalay);
		}
		Serial.print("-");
		delay(100);
		for (serverPos = servoMin; serverPos >= servoMax; serverPos -= 1) {
			servo.write(serverPos);
			delay(servoDalay);
		}
	}
	Serial.println(" done");
	return amount;
}



void setup() {
	// initialize serial communications at 115200 bps:
	Serial.begin(115200);
	delay(1000);

	// We start by connecting to a WiFi network
	Serial.println();
	Serial.println();
	Serial.println("/***********************************************************************************************");
	Serial.println(" ** Payout v0.0.1");
	Serial.println(" ***********************************************************************************************/");

	servo.attach(servoPin);  // attaches the servo on GIO2 to the servo object
	servo.write(servoMax);
}

void loop() {
	// check if data has been sent from the computer:
	if (Serial.available()) {
		// read the most recent byte (which will be from 0 to 255):
		payoutAmount = Serial.parseInt();
		Serial.print("Payout ");
		Serial.println(payoutAmount);
		payout(payoutAmount);
	}
}