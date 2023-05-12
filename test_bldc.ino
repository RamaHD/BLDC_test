#include <SimpleFOC.h>

// DRV8302 pins connections
// don't forget to connect the common ground pin
#define   INH_A 9
#define   INH_B 10
#define   INH_C 11
#define   EN_GATE 8
#define   M_PWM 6
#define   M_OC 5
#define   OC_ADJ 7

//target variable
float target_velocity = 0;

// motor instance
BLDCMotor motor = BLDCMotor(23);

// driver instance
BLDCDriver3PWM driver = BLDCDriver3PWM(INH_A, INH_B, INH_C, EN_GATE);

// instantiate the commander
//Commander command = Commander(Serial);
//void doTarget(char* cmd) { command.scalar(&target_velocity, cmd); }
//void doLimit(char* cmd) { command.scalar(&motor.voltage_limit, cmd); }

void setup() {

  pinMode(M_OC, OUTPUT);
  digitalWrite(M_OC, LOW);

  pinMode(M_PWM, OUTPUT);
  digitalWrite(M_PWM, HIGH);

  pinMode(OC_ADJ, OUTPUT);
  digitalWrite(OC_ADJ, HIGH);

  // driver config
  // power supply voltage [V]
  driver.voltage_power_supply = 40;
  // limit the maximal dc voltage the driver can set
  // as a protection measure for the low-resistance motors
  // this value is fixed on startup
  driver.voltage_limit = 39;
  driver.init();
  // link the motor and the driver
  motor.linkDriver(&driver);

  // limiting motor movements
  // limit the voltage to be set to the motor
  // start very low for high resistance motors
  // currnet = resistance*voltage, so try to be well under 1Amp
  motor.voltage_limit = 46;   // [V]
  motor.velocity_limit = 2000;

  // open loop control config
  motor.controller = MotionControlType::velocity_openloop;

  // init motor hardware
  motor.init();

  // add target command T
  //  command.add('T', doTarget, "target velocity");
  //  command.add('L', doLimit, "voltage limit");

  Serial.begin(115200);
  Serial.println("Motor ready!");
  Serial.println("Set target velocity [rad/s]");
  _delay(1000);
}

void loop() {
  int Val = analogRead(A2); // memasukan nilai pembacaan analog pin potensio ke variabel sensorValue
  int val = map(Val, 170, 874, 0, 42);
  // open loop velocity movement
  // using motor.voltage_limit and motor.velocity_limit
  motor.move(val);
  Serial.println(Val);

  // user communication
  //  command.run();
}
