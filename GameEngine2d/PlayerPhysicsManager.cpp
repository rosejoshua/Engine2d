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
    maxHorizontalVelocity = 0.0f;
}

PlayerPhysicsManager::~PlayerPhysicsManager()
{
}

void PlayerPhysicsManager::setModifiers(int playerHeight)
{
    jumpVelocity = playerHeight / -40.0f;
    horVelModPlayerRunSprint = playerHeight / 1000.0f;
    horVelModPlayerWalk = playerHeight / 4000.0f;
    gravityModifier = playerHeight / 9000.0f;
    maxWalkVelocity = playerHeight / 170.0f;
    maxRunVelocity = playerHeight / 70.0f;
    maxSprintVelocity = playerHeight / 40.0f;
    maxHorizontalVelocity = playerHeight / 30.0;
}

void PlayerPhysicsManager::updatePlayerVelocities(ControlsManager *controlsManager)
{
    //todo:Replace with heavy gravity that doesnt depend on negative velocity

    if ((!controlsManager->button4Down) && (!controlsManager->canJump) && yVelocity < 0)
    {
        yVelocity /= 0.25 * (float)(SDL_GetTicks64() - lastPhysicsUpdate);
    }
    
    if (controlsManager->button4Down && controlsManager->canJump) 
    {
        yVelocity += jumpVelocity;
        controlsManager->canJump = false;
    }

    //move player position on x axis based on xVelocity        
    if (controlsManager->xDir < 0 && controlsManager->xDirLast <= 0)
    {
        if (controlsManager->button8Down) {
            if (xVelocity > -maxSprintVelocity)
            {
                xVelocity -= horVelModPlayerRunSprint;
                if (xVelocity < -maxSprintVelocity)
                {
                    xVelocity = -maxSprintVelocity;
                }
            }
        }
        else if (controlsManager->xDir == -2)
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
    else if (controlsManager->xDir > 0 && controlsManager->xDirLast >= 0)
    {
        if (controlsManager->button8Down) {
            if (xVelocity < maxSprintVelocity)
            {
                xVelocity += horVelModPlayerRunSprint;
                if (xVelocity > maxSprintVelocity)
                {
                    xVelocity = maxSprintVelocity;
                }
            }
        }
        else if (controlsManager->xDir == 2)
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

