#pragma once

#include <atomic>
#include "drive.hpp"
#include "Actions.h"

#include <vector>

/// @brief This class is meant for deplacement control, it's an abstraction to provide a more action like movement
/// like move forward of x meter, do a rotation x degrees, etc..
/// It's also meant for (in the future) a more spatial movement, PAMI think it's a specific position and will try to reach pos Y
/// The core motor is done by the driver drive.h 
class DeplacementControl
{
    public:
    /// this enum represent the current state of the objectif of the deplacement
    // are we running, are we block, etc...
    enum DeplacementControlState : uint8_t
    {
        DCS_IDLE = 0,
        DCS_RUNNING,
        DCS_BLOCKED,

        DCS_UNKNOW // should not happen
    };
    
    int mId; // represent the id of the PAMI
private:
    DRIVE *mDrive;
    std::atomic<DeplacementControlState> mState;

    std::atomic<bool> mEnd;
    std::atomic<bool> mIsRunning;
    std::atomic<bool> mIsStopped;
    /* data */

    double mLength;       // Length covered by the motor
    double mMotorSpeed;                // Speed of the motor
    double mTargetLength;              // Target length to reach

public :
    std::mutex mMutexObjectiveAction;
    std::vector<std::shared_ptr<Action>> mObjectivesAction;
public:
    DeplacementControl();
    virtual ~DeplacementControl();

    /// basic Action ///
    // simple oneshot action that the pami will do
    void stop();
    void resume();
    void end();

    /// objectif action ///
    // objectif action a followup of step that the pami want to achieve
    void startStrat_simpleLinesToFlower();

    /// infos function ///
    // provide informations (some mutexed) about ongoing actions
    inline DeplacementControlState getDeplacementControlState() {return mState.load();}
    inline DRIVE::DRIVE_STATE getDriverDriveState() { return mDrive->getDriveState(); }

    void setDrive(DRIVE *drive);
    void setId(int id);

    void testRun(int durationSeconds);

    void loop_motor_drive();
    void setReady();
    void startRunning();

private:
    int getDelta(signed long long &delta_left, signed long long &delta_right);
    void getDirectionFactor(signed long long &delta_left, signed long long &delta_right);

    void getDefaultMotorSpeed(signed long &left_motor, signed long &right_motor);

    /// Action related function ///
public:
    std::shared_ptr<Action> getNextAction();
    void addAction(std::shared_ptr<Action> action);
    void clearAction();

    inline bool isRunning(){ return mIsRunning.load(); }
    inline bool isStopped(){ return mIsStopped.load(); }

    // should come later
    void addPriorityAction(std::shared_ptr<Action> action);

    void doCalibrationOnMotorSpeed();
    

private :
    void validateAction();
    void skipAction();

    /// should come in the future
    void compomiseCurrentImpossibleAction();
    void motorSpeedCalibration();

};
