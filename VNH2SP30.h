#ifndef VNH2SP30_H
#define VNH2SP30_H

#include <stdint.h>

//Four possible states of the H-Bridge
//clockwise: foward
//counterclockwise: backward
//braketovcc: brake
//braketognc: brake
typedef enum motorState
{
    CLOCKWISE,
    COUNTERCLOCKWISE,
    BRAKETOVCC,
    BRAKETOGND //default, safer for the battery
} MotorState;

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct VNH2SP30 VNH2SP30;

    VNH2SP30 *VNH2SP30__create(
        uint8_t inA, uint8_t inB, uint8_t pwm,
        uint8_t en, uint8_t cs); //constructor

    void VNH2SP30__end(VNH2SP30 *self); //destructor

    //member functions
    bool VNH2SP30__motorDriverIsFaulty(VNH2SP30 *self);

    unsigned int VNH2SP30__checkMotorCurrentDraw(VNH2SP30 *self);

    void VNH2SP30__turnOffMotor(VNH2SP30 *self);

    void VNH2SP30__turnOnMotor(VNH2SP30 *self, MotorState state);

    MotorState VNH2SP30__getMotorState(VNH2SP30 *self);

#ifdef __cplusplus
}
#endif

#endif