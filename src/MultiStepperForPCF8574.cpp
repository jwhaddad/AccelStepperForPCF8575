// MultiStepperForPCF8574.cpp
//
// Copyright (C) 2015 Mike McCauley
// $Id: MultiStepperForPCF8574.cpp,v 1.3 2020/04/20 00:15:03 mikem Exp mikem $

#include "MultiStepperForPCF8574.h"
#include "AccelStepperForPCF8574.h"

MultiStepperForPCF8574::MultiStepperForPCF8574()
    : _num_steppers(0)
{
}

boolean MultiStepperForPCF8574::addStepper(AccelStepperForPCF8574& stepper)
{
    if (_num_steppers >= MULTISTEPPER_MAX_STEPPERS)
	return false; // No room for more
    _steppers[_num_steppers++] = &stepper;
    return true;
}

void MultiStepperForPCF8574::moveTo(long absolute[])
{
    // First find the stepper that will take the longest time to move
    float longestTime = 0.0;

    uint8_t i;
    for (i = 0; i < _num_steppers; i++)
    {
	long thisDistance = absolute[i] - _steppers[i]->currentPosition();
	float thisTime = abs(thisDistance) / _steppers[i]->maxSpeed();

	if (thisTime > longestTime)
	    longestTime = thisTime;
    }

    if (longestTime > 0.0)
    {
	// Now work out a new max speed for each stepper so they will all 
	// arrived at the same time of longestTime
	for (i = 0; i < _num_steppers; i++)
	{
	    long thisDistance = absolute[i] - _steppers[i]->currentPosition();
	    float thisSpeed = thisDistance / longestTime;
	    _steppers[i]->moveTo(absolute[i]); // New target position (resets speed)
	    _steppers[i]->setSpeed(thisSpeed); // New speed
	}
    }
}

// Returns true if any motor is still running to the target position.
boolean MultiStepperForPCF8574::run()
{
    uint8_t i;
    boolean ret = false;
    for (i = 0; i < _num_steppers; i++)
    {
	if ( _steppers[i]->distanceToGo() != 0)
	{
	    _steppers[i]->runSpeed();
	    ret = true;
	}
	// Caution: it has een reported that if any motor is used with acceleration outside of
	// MultiStepperForPCF8574, this code is necessary, you get 
	// strange results where it moves in the wrong direction for a while then 
	// slams back the correct way.
#if 0
	else
	{
	    // Need to call this to clear _stepInterval, _speed and _n 
	    otherwise future calls will fail.
		_steppers[i]->setCurrentPosition(_steppers[i]->currentPosition());
	}
#endif
	
    }
    return ret;
}

// Blocks until all steppers reach their target position and are stopped
void    MultiStepperForPCF8574::runSpeedToPosition()
{ 
    while (run())
	;
}

