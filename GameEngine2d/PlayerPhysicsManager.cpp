#include "PlayerPhysicsManager.hpp"

PlayerPhysicsManager::PlayerPhysicsManager()
{
    jumpVelocity = 0.0f;
    jumpVelocityInWater = 0.0f;
    maxUpwardVelocityInWater = 0.0f;
    maxDownwardVelocityInWater = 0.0f;
    horVelModPlayerRunSprint = 0.0f;
    horVelModPlayerWalk = 0.0f;
    gravityModifier = 0.0f;
    gravityModifierInWater = 0.0f;
    maxWalkVelocity = 0.0f;
    maxRunVelocity = 0.0f;
    maxSprintVelocity = 0.0f;
    maxUpwardVerticalVelocity - 0.0f;
    maxDownwardVerticalVelocity - 0.0f;
    hitTheBrakes = 0.25;
    yVelocity = 0.0f;
    xVelocity = 0.0f;
    currentSpecialBehavior = 0;
    inWater = false;
    lastPhysicsUpdate = 0;
}

PlayerPhysicsManager::~PlayerPhysicsManager()
{
}

void PlayerPhysicsManager::setModifiers(int playerHeight)
{
    jumpVelocity = playerHeight / -35.0f;
    jumpVelocityInWater = playerHeight / -160.0f;
    maxUpwardVelocityInWater = playerHeight / -90.0f;
    maxDownwardVelocityInWater = playerHeight / 70.0f;
    horVelModPlayerRunSprint = playerHeight / 1500.0f;
    horVelModPlayerWalk = playerHeight / 4000.0f;
    gravityModifier = playerHeight / 9000.0f;
    gravityModifierInWater = gravityModifier / 9.0f;
    maxWalkVelocity = playerHeight / 170.0f;
    maxRunVelocity = playerHeight / 80.0f;
    maxSprintVelocity = playerHeight / 60.0f;
    maxUpwardVerticalVelocity = -playerHeight / 15.0f;
    maxDownwardVerticalVelocity = playerHeight / 15.0f;
}

void PlayerPhysicsManager::updatePlayerVelocities(ControlsManager &controlsManager, Mix_Chunk* jumpSound, Mix_Chunk* thumpSound)
{
    if (!inWater)
    {
        if (currentSpecialBehavior == 1)
        {
            yVelocity = maxUpwardVerticalVelocity;
            Mix_PlayChannel(3, thumpSound, 0);
            currentSpecialBehavior = 2;
        }
        else if (currentSpecialBehavior == 3)
        {
            yVelocity = maxDownwardVerticalVelocity;
            Mix_PlayChannel(3, thumpSound, 0);
            currentSpecialBehavior = 2;
        }
        else if (currentSpecialBehavior == 4)
        {
            xVelocity = maxDownwardVerticalVelocity;
            Mix_PlayChannel(3, thumpSound, 0);
            currentSpecialBehavior = 5;
        }
        else if (currentSpecialBehavior == 6)
        {
            xVelocity = maxUpwardVerticalVelocity;
            Mix_PlayChannel(3, thumpSound, 0);
            currentSpecialBehavior = 5;
        }

        //add jump to velocity
        else if (controlsManager.button0Down && controlsManager.canJump)
        {
            yVelocity += jumpVelocity;
            if (yVelocity < maxUpwardVerticalVelocity)
                yVelocity = maxUpwardVerticalVelocity;
            controlsManager.canJump = false;
            Mix_PlayChannel(2, jumpSound, 0);
        }
        else if (!controlsManager.button0Down && !controlsManager.canJump && yVelocity < 0)
        {
            if (currentSpecialBehavior == 0)
                yVelocity /= 0.25 * (float)(SDL_GetTicks64() - lastPhysicsUpdate);
        }

        //move player position on x axis based on xVelocity       
        if (currentSpecialBehavior !=5)
        {
            if (xVelocity < -maxSprintVelocity || xVelocity > maxSprintVelocity)
            {
                //xVelocity /= hitTheBrakes * (float)(SDL_GetTicks64() - lastPhysicsUpdate);
                xVelocity /= (1.0 + 0.01 * (float)(SDL_GetTicks64() - lastPhysicsUpdate));

                //if ((controlsManager.xDir > 0 && xVelocity < -maxSprintVelocity) || (controlsManager.xDir < 0 && xVelocity > maxSprintVelocity))
                //    xVelocity /= hitTheBrakes * (float)(SDL_GetTicks64() - lastPhysicsUpdate);
                    //xVelocity /= (1.0 + 0.01 * (float)(SDL_GetTicks64() - lastPhysicsUpdate));
            }

            if (controlsManager.xDir < 0 && controlsManager.xDirLast <= 0)
            {
                //if (xVelocity > maxSprintVelocity)
                //{
                //    xVelocity /= hitTheBrakes * (float)(SDL_GetTicks64() - lastPhysicsUpdate);
                //}
                if (controlsManager.button2Down) {
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
                //if (xVelocity < -maxSprintVelocity)
                //{
                //    xVelocity /= hitTheBrakes * (float)(SDL_GetTicks64() - lastPhysicsUpdate);
                //}
                if (controlsManager.button2Down)
                {
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
            //else if ()
            //{

            //}
            //else if ()
            //{

            //}
            else
            {
                xVelocity /= hitTheBrakes * (float)(SDL_GetTicks64() - lastPhysicsUpdate);
            }
        }
    }
    //in water physics
    else
    {
        //add jump to velocity
        if (controlsManager.button0Down && controlsManager.canJump)
        {
            yVelocity += jumpVelocityInWater;
            if (yVelocity < maxUpwardVelocityInWater)
                yVelocity = maxUpwardVelocityInWater;
            controlsManager.canJump = false;
        }
        //need timer to prevent jump spams
        if (!controlsManager.button0Down)
        {
            controlsManager.canJump = true;
        }

        //move player position on x axis based on xVelocity        
        if (controlsManager.xDir < 0 && controlsManager.xDirLast <= 0)
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
        else if (controlsManager.xDir > 0 && controlsManager.xDirLast >= 0)
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
        else
        {
            xVelocity /= hitTheBrakes * (float)(SDL_GetTicks64() - lastPhysicsUpdate);
        }
    }
}

//void PlayerPhysicsManager::updatePlayerVelocitiesSpecial(ControlsManager& controlsManager, int behavior)
//{
//    if (behavior == 1)
//    {
//        yVelocity += -maxUpwardVerticalVelocity;
//        if (yVelocity < -maxUpwardVerticalVelocity)
//            yVelocity = -maxUpwardVerticalVelocity;
//        controlsManager.canJump = false;
//        //Mix_PlayChannel(2, jumpSound, 0);
//    }
//}

