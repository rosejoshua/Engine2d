#include "PlayerPhysicsManager.hpp"

PlayerPhysicsManager::PlayerPhysicsManager()
{
    jumpVelocity = 0.0f;
    horVelModPlayerRunSprint = 0.0f;
    horVelModPlayerWalk = 0.0f;
    gravityModifier = 0.0f;
    maxWalkVelocity = 0.0f;
    maxRunVelocity = 0.0f;
    maxSprintVelocity = 0.0f;
    yVelocity = 0.0f;
    xVelocity = 0.0f;
    lastPhysicsUpdate = 0;
}

PlayerPhysicsManager::~PlayerPhysicsManager()
{
}

void PlayerPhysicsManager::setModifiers(int playerHeight)
{
    jumpVelocity = playerHeight / -40.0f;
    horVelModPlayerRunSprint = playerHeight / 1000.0;
    horVelModPlayerWalk = playerHeight / 4000.0;
    gravityModifier = playerHeight / 9000.0;
    maxWalkVelocity = playerHeight / 170.0;
    maxRunVelocity = playerHeight / 80.0;
    maxSprintVelocity = playerHeight / 50.0;
}

void PlayerPhysicsManager::updatePlayerVelocities(ControlsManager &controlsManager)
{
    //add jump to velocity
    if (controlsManager.button4Down && controlsManager.canJump) {
        yVelocity += jumpVelocity;
        controlsManager.canJump = false;
    }
    else if (!controlsManager.button4Down && !controlsManager.canJump && yVelocity < 0)
    {
        yVelocity /= 0.25 * (float)(SDL_GetTicks64() - lastPhysicsUpdate);
    }

    //move player position on x axis based on xVelocity        
    if (controlsManager.xDir < 0 && controlsManager.xDirLast <= 0)
    {
        if (controlsManager.button8Down) {
            if (xVelocity > -maxSprintVelocity)
            {
                xVelocity -= horVelModPlayerRunSprint;
                if (xVelocity < -maxSprintVelocity)
                {
                    xVelocity = -maxSprintVelocity;
                }
            }
        }
        else if (controlsManager.xDir == -2)
        {
            if (xVelocity > -maxRunVelocity)
            {
                xVelocity -= horVelModPlayerRunSprint;
                if (xVelocity < -maxRunVelocity)
                {
                    xVelocity = -maxRunVelocity;
                }
            }
            else
            {
                xVelocity = -maxRunVelocity;
            }
        }
        else
        {
            if (xVelocity > -maxWalkVelocity)
            {
                xVelocity -= horVelModPlayerWalk;
                if (xVelocity < -maxWalkVelocity)
                {
                    xVelocity = -maxWalkVelocity;
                }
            }
            else
            {
                xVelocity = -maxWalkVelocity;
            }
        }
    }
    else if (controlsManager.xDir > 0 && controlsManager.xDirLast >= 0)
    {
        if (controlsManager.button8Down) {
            if (xVelocity < maxSprintVelocity)
            {
                xVelocity += horVelModPlayerRunSprint;
                if (xVelocity > maxSprintVelocity)
                {
                    xVelocity = maxSprintVelocity;
                }
            }
        }
        else if (controlsManager.xDir == 2)
        {
            if (xVelocity < maxRunVelocity)
            {
                xVelocity += horVelModPlayerRunSprint;
                if (xVelocity > maxRunVelocity)
                {
                    xVelocity = maxRunVelocity;
                }
            }
            else
            {
                xVelocity = maxRunVelocity;
            }
        }
        else
        {
            if (xVelocity < maxWalkVelocity)
            {
                xVelocity += horVelModPlayerWalk;
                if (xVelocity > maxWalkVelocity)
                {
                    xVelocity = maxWalkVelocity;
                }
            }
            else
            {
                xVelocity = maxWalkVelocity;
            }
        }
    }
    else {
        xVelocity /= 0.25 * (float)(SDL_GetTicks64() - lastPhysicsUpdate);
    }
}

