#include "MovementAction.h"
#include "drive.hpp"

MovementAction::MovementAction() : mSpeedRight(DEFAULT_MOTOR_SPEED_PAMI_ONE_WHEEL_RIGHT),
                                   mSpeedLeft(DEFAULT_MOTOR_SPEED_PAMI_ONE_WHEEL_LEFT),
                                   mPosX(0),
                                   mPosY(0),
                                   mLegacyMotorPosRight(0),
                                   mLegacyMotorPosLeft(0)
{
}

MovementAction::MovementAction(signed long speedR,
                               signed long speedL,
                               signed long posX,
                               signed long posY,
                               signed long motorPosR /*  = 0 */,
                               signed long motorPosL /*  = 0 */) : mSpeedRight(speedR),
                                                                   mSpeedLeft(speedL),
                                                                   mPosX(posX),
                                                                   mPosY(posY),
                                                                   mLegacyMotorPosRight(motorPosR),
                                                                   mLegacyMotorPosLeft(motorPosL)
{
    printf("MovementAction::MovementAction() left:%ld / %ld, right:%ld / %ld \n",
    mPosX, mPosY,
    mLegacyMotorPosRight, mLegacyMotorPosLeft);
}

MovementAction::MovementAction(const MovementAction &ma) : mSpeedRight(ma.mSpeedRight),
                                                           mSpeedLeft(ma.mSpeedLeft),
                                                           mPosX(ma.mPosX),
                                                           mPosY(ma.mPosY),
                                                           mLegacyMotorPosRight(ma.mLegacyMotorPosRight),
                                                           mLegacyMotorPosLeft(ma.mLegacyMotorPosLeft)
{
        printf("MovementAction::MovementAction(copy) left:%ld / %ld, right:%ld / %ld \n",
    mPosX, mPosY,
    mLegacyMotorPosRight, mLegacyMotorPosLeft);
}

void MovementAction::printValue() const 
{
    printf("MovementAction::printValue() posy:%ld / posY:%ld, right:%ld / left:%ld \n",
    mPosX, mPosY,
    mLegacyMotorPosRight, mLegacyMotorPosLeft);
}

MovementAction::~MovementAction()
{
}
Action::TYPE_OF_ACTION MovementAction::getTypeAction() const
{
    return TYPE_OF_ACTION::MOVEMENT;
};

bool MovementAction::isActionValid(void *arg) const
{
    if (arg == nullptr)
    {
        printf("MovementAction::isActionValid(arg == nullptr)\n");
        return true;
    }
    DRIVE *dr = dynamic_cast<DRIVE *>(static_cast<DRIVE *>(arg)); //(DRIVE *)arg;//
    if (dr == nullptr)
    {
        throw std::runtime_error("MovementAction isActionValid Object is not of the desired type!");
    }

    // todo: legacy code !, should be changed for position
    if ((dr->left.position * mLegacyMotorPosLeft) > 0 &&
        (dr->right.position * mLegacyMotorPosRight) > 0 &&
        abs(dr->left.position - mLegacyMotorPosLeft) <= MOTOR_POSITION_TOLERANCE &&
        abs(dr->right.position - mLegacyMotorPosRight) <= MOTOR_POSITION_TOLERANCE)
    {
        printf("MovementAction::isActionValid(true)\n");
        return true;
    }

    printf("MovementAction::isActionValid(false) left:%lld / right:%lld, poleft:%ld / poright:%ld \n",
    dr->left.position,
    dr->right.position, 
    mLegacyMotorPosLeft,
    mLegacyMotorPosRight);
    return false;
}

void MovementAction::setMoveTo(signed long posX, signed long posY)
{
    mPosX = posX;
    mPosY = mPosY;
}

void MovementAction::setMotorPosTo(signed long posRight, signed long posLeft)
{
    mLegacyMotorPosRight = posRight;
    mLegacyMotorPosLeft = posLeft;
    printf("MovementAction::setMotorPosTo(left:%ld - right:%d)\n",mLegacyMotorPosLeft, mLegacyMotorPosRight);
}


//////////////
/// Static ///
//////////////
std::shared_ptr<Action> MovementAction::createActionGoForward() {
    printf("MovementAction::createActionGoForward()\n");
    return std::make_shared<MovementAction>(DEFAULT_MOTOR_SPEED_PAMI_ONE_WHEEL_LEFT,
    DEFAULT_MOTOR_SPEED_PAMI_ONE_WHEEL_RIGHT,
    0, 0, 720000, 720000);
}

std::shared_ptr<Action> MovementAction::createActionGoBackward()
{

}

std::shared_ptr<Action> MovementAction::createActionTurn90Right()
{
}

std::shared_ptr<Action> MovementAction::createActionTurn90Left()
{
}

std::shared_ptr<Action> MovementAction::createActionTurn180()
{
}
