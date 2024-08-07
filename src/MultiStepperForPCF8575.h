// MultiStepperForPCF8575.h

#ifndef MultiStepperForPCF8575_h
#define MultiStepperForPCF8575_h

#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

#define MULTISTEPPER_MAX_STEPPERS 10

class AccelStepperForPCF8575;

/////////////////////////////////////////////////////////////////////
/// \class MultiStepperForPCF8575 MultiStepperForPCF8575.h <MultiStepperForPCF8575.h>
/// \brief Operate multiple AccelStepperForPCF8575s in a co-ordinated fashion
///
/// This class can manage multiple AccelStepperForPCF8575s (up to MULTISTEPPER_MAX_STEPPERS = 10), 
/// and cause them all to move
/// to selected positions at such a (constant) speed that they all arrive at their
/// target position at the same time. This can be used to support devices with multiple steppers
/// on say multiple axes to cause linear diagonal motion. Suitable for use with X-Y plotters, flatbeds,
/// 3D printers etc
/// to get linear straight line movement between arbitrary 2d (or 3d or ...) positions.
///
/// Caution: only constant speed stepper motion is supported: acceleration and deceleration is not supported
/// All the steppers managed by MultiStepperForPCF8575 will step at a constant speed to their
/// target (albeit perhaps different speeds for each stepper).
class MultiStepperForPCF8575
{
public:
    /// Constructor
    MultiStepperForPCF8575();

    /// Add a stepper to the set of managed steppers
    /// There is an upper limit of MULTISTEPPER_MAX_STEPPERS = 10 to the number of steppers that can be managed
    /// \param[in] stepper Reference to a stepper to add to the managed list
    /// \return true if successful. false if the number of managed steppers would exceed MULTISTEPPER_MAX_STEPPERS
    boolean addStepper(AccelStepperForPCF8575& stepper);

    /// Set the target positions of all managed steppers 
    /// according to a coordinate array.
    /// New speeds will be computed for each stepper so they will all arrive at their 
    /// respective targets at very close to the same time.
    /// \param[in] absolute An array of desired absolute stepper positions. absolute[0] will be used to set
    /// the absolute position of the first stepper added by addStepper() etc. The array must be at least as long as 
    /// the number of steppers that have been added by addStepper, else results are undefined.
    void moveTo(long absolute[]);
    
    /// Calls runSpeed() on all the managed steppers
    /// that have not acheived their target position.
    /// \return true if any stepper is still in the process of running to its target position.
    boolean run();

    /// Runs all managed steppers until they acheived their target position.
    /// Blocks until all that position is acheived. If you dont
    /// want blocking consider using run() instead.
    void    runSpeedToPosition();
    
private:
    /// Array of pointers to the steppers we are controlling.
    /// Fills from 0 onwards
    AccelStepperForPCF8575* _steppers[MULTISTEPPER_MAX_STEPPERS];

    /// Number of steppers we are controlling and the number
    /// of steppers in _steppers[]
    uint8_t       _num_steppers;
};

/// @example MultiStepperForPCF8575.pde
/// Use MultiStepperForPCF8575 class to manage multiple steppers and make them all move to 
/// the same position at the same time for linear 2d (or 3d) motion.

#endif
