#include "Arduino.h"
#include "assert.h"

//creating a struct VHN2SP30 that contains
//all the pins on the VNH2SP30 breakout
//and two config values
typedef struct VNH2SP30
{
    uint8_t inA;      //clockwise input (DIGITAL)
    uint8_t inB;      //counterclockwise input (DIGITAL)
    uint8_t pwm;      //pwm input (DIGITAL)
    uint8_t en;       //enable in-out (ANALOG)
    uint8_t cs;       //current sense output (ANALOG)
    MotorState state; //state of the motor
} VNH2SP30;

//Function to initialize the differents variables
void VNH2SP30__init(VNH2SP30 *self,
                    uint8_t inA,
                    uint8_t inB,
                    uint8_t pwm,
                    uint8_t en,
                    uint8_t cs)
{
    self->inA = inA;
    self->inB = inB;
    self->pwm = pwm;
    self->en = en;
    self->cs = cs;
    self->state = BRAKETOGND;
}

//Function to create a new instance of the VNH2SP30 struct
VNH2SP30 *VNH2SP30__create(uint8_t inA,
                           uint8_t inB,
                           uint8_t pwm,
                           uint8_t en,
                           uint8_t cs)
{
    //allocate memory
    VNH2SP30 *md = (VNH2SP30 *)malloc(sizeof(VNH2SP30));
    assert(md);

    //initialize VNH2SP30 with variables
    VNH2SP30__init(md, inA, inB, pwm, en, cs, mode);

    //map Arduino to VNH2SP30 breakout
    pinMode(md->inA, OUTPUT);
    pinMode(md->pwm, OUTPUT);
    pinMode(md->cs, INPUT);
    digitalWrite(md->inA, LOW);
    pinMode(md->inB, OUTPUT);
    digitalWrite(md->inB, LOW);

    return md;
}

//Function to reset clean
void VNH2SP30__reset(VNH2SP30 *self)
{
}

//Function to remove all allocated memory
void VNH2SP30__end(VNH2SP30 *md)
{
    if (md) //if the allocated memory !null
    {
        VNH2SP30__reset(md);
        free(md);
    }
}

//Function to check VNH2SP30 current status
//gives true if an hardware problem is detected, false otherwise
bool VNH2SP30__motorDriverIsFaulty(VNH2SP30 *self)
{
    //the enable pin is normally on HIGH but of there
    //is an hardware problem, it will pull to LOW
    (digitalRead(self->en) == LOW) ? true : false;
}

//Function that check the current motor draw
//gives a value between 0 and 1024
//TODO: map to AMPS
unsigned int VNH2SP30__checkMotorCurrentDraw(VNH2SP30 *self)
{
    return analogRead(self->cs);
}

//Function to turn off the motor
void VNH2SP30__turnOffMotor(VNH2SP30 *self)
{
    digitalWrite(self->inA, LOW);
    digitalWrite(self->inB, LOW);

    analogWrite(self->pwm, 0);
    self->state = BRAKETOGND;
}

//Function to make motor operate in all states (drive, reverse, brake to ground and brake to positive)
void VNH2SP30__turnOnMotor(VNH2SP30 *self, MotorState state)
{
    //set the pins states
    (state == CLOCKWISE || state == BRAKETOVCC) ? digitalWrite(self->inA, HIGH) : digitalWrite(self->inA, LOW);
    (state == COUNTERCLOCKWISE || state == BRAKETOVCC) ? digitalWrite(self->inB, HIGH) : digitalWrite(self->inB, LOW);

    self->state = state;
}

//Function to check operating state of the VNH2SP30 driver
MotorState VNH2SP30__getMotorState(VNH2SP30 *self)
{
    return self->state;
}
