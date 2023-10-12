/**
 * @file CodeReleaseKickAtGoalCard.cpp
 *
 * This file implements a basic striker behavior for the code release.
 *
 * @author Arne Hasselbring
 */
/*
#define R1X -4500
#define R1Y 0

#define R3X -2000
#define R3Y 1200 // -1200

#define R2X -1200
#define R2Y 0

#define R4X -2000
#define R4Y -1200
*/
#define R1X theFieldDimensions.xPosOwnGroundLine
#define R1Y 0

#define R3X -1300
#define R3Y 700 // 1200

#define R4X -700
#define R4Y 0

#define R2X -1300
#define R2Y -700

#include "Representations/Communication/RobotInfo.h"
#include "Representations/BehaviorControl/TeamBehaviorStatus.h"
#include "Representations/Communication/GameInfo.h"
#include "Representations/Communication/TeamInfo.h"
#include "Representations/Configuration/BallSpecification.h"
#include "Representations/BehaviorControl/FieldBall.h" 
#include "Representations/BehaviorControl/Skills.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Modeling/RobotPose.h"
#include "Tools/BehaviorControl/Framework/Card/Card.h"
#include "Tools/BehaviorControl/Framework/Card/CabslCard.h"
#include "Tools/Math/BHMath.h"
#include "Modules/Modeling/BallInGoalTracker/BallInGoalTracker.h"

CARD(CodeReleaseKickAtGoalCard,
{,
  CALLS(Activity),
  CALLS(GoToBallAndKick),
  CALLS(LookForward),
  CALLS(Stand),
  CALLS(Say),
  CALLS(LookActive),
  CALLS(LookAtPoint),
  CALLS(WalkAtRelativeSpeed),
  CALLS(WalkToKickoffPose),

  CALLS(InterceptBall),
  // CALLS(AfterInterceptBall),
  
  
  //REQUIRES(BallInGoalTracker),
  REQUIRES(OpponentTeamInfo),
  REQUIRES(BallSpecification),
  REQUIRES(GameInfo),
  REQUIRES(OwnTeamInfo),
  REQUIRES(RobotPose),
  REQUIRES(TeamBehaviorStatus),
  REQUIRES(FieldBall),
  REQUIRES(FieldDimensions),
  REQUIRES(RobotInfo),


  DEFINES_PARAMETERS(
  {,
    (float)(1.f) walkSpeed,
    (int)(1000) initialWaitTime,
    (int)(7000) ballNotSeenTimeout,
  }), 
});

class CodeReleaseKickAtGoalCard : public CodeReleaseKickAtGoalCardBase
{
  bool preconditions() const override
  {
    return true;
  }

  bool postconditions() const override
  {
    return true;
  }

  option
  {
    theActivitySkill(BehaviorStatus::codeReleaseKickAtGoal);

    initial_state(start)
    {
      transition
      {
        if(state_time > initialWaitTime)
        {
          if(theRobotInfo.number == 1)
          {
            goto goalKeeper;
          }
          /*if(theRobotInfo.number == 2)
          {
            goto supporter2;
          }
          if(theRobotInfo.number == 3)
          {
            goto supporter3;
          }*/
          if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))
          {
            goto searchForBall;
          }
          if(theGameInfo.setPlay == SET_PLAY_NONE)
          {
            goto goToBallAndKick;
          }
          if(theGameInfo.setPlay == SET_PLAY_PUSHING_FREE_KICK)
          {
            if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
            {
              goto PushingFreeKick;
            }
            else
            {
              goto BuildWall;
            }
          }
          else if(theGameInfo.setPlay == SET_PLAY_KICK_IN)
          {
            if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
            {
              goto KickIn;
            }
            else
            {
              goto BuildWall;
            }
          }
          else if(theGameInfo.setPlay == SET_PLAY_CORNER_KICK)
          {
            if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
            {
              goto CornerKickToOwnRobot;
            }
            else
            {
              goto BuildWall;
            }
          }
          else if(theGameInfo.setPlay == SET_PLAY_GOAL_KICK)
          {
            if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
            {
              goto GoalFreeKick;
            }
            else
            {
              goto BuildWall;
            }
          }
        }
          
      }

      action
      {
        theLookForwardSkill();
        theStandSkill();
      }
    }
/*======================================================================================================*/
    state(goToBallAndKick)
    {
      transition
      {
        if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))
        {
          goto searchForBall;
        }
        if(theGameInfo.setPlay == SET_PLAY_PUSHING_FREE_KICK)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto PushingFreeKick;
          }
          else
          {
            goto BuildWall;
          }
        }
        else if(theGameInfo.setPlay == SET_PLAY_KICK_IN)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto KickIn;
          }
          else
          {
            goto BuildWall;
          }
        }
        else if(theGameInfo.setPlay == SET_PLAY_CORNER_KICK)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto CornerKickToOwnRobot;
          }
          else
          {
            goto BuildWall;
          }
        }
        else if(theGameInfo.setPlay == SET_PLAY_GOAL_KICK)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto GoalFreeKick;
          }
          else
          {
            goto BuildWall;
          }
        }
      }

      action
      {
        theGoToBallAndKickSkill(calcAngleToGoal(), KickInfo::walkForwardsLeftLong);
      }
    }
/*======================================================================================================*/
    state(searchForBall)
    {
      transition
      {
        if(theFieldBall.ballWasSeen())
        {
          if(theGameInfo.setPlay == SET_PLAY_NONE)
          {
            goto goToBallAndKick;
          }
          else if(theGameInfo.setPlay == SET_PLAY_PUSHING_FREE_KICK)
          {
            if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
            {
              goto PushingFreeKick;
            }
            else
            {
              goto BuildWall;
            }
          }
          else if(theGameInfo.setPlay == SET_PLAY_GOAL_KICK)
          {
            if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
            {
              goto GoalFreeKick;
            }
            else
            {
              goto BuildWall;
            }
          }
          else if(theGameInfo.setPlay == SET_PLAY_KICK_IN)
          {
            if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
            {
              goto KickIn;
            }
            else
            {
              goto BuildWall;
            }
          }
          else if(theGameInfo.setPlay == SET_PLAY_CORNER_KICK)
          {
            if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
            {
              goto CornerKickToOwnRobot;
            }
            else
            {
              goto BuildWall;
            }
          }
        }
      }

      action
      {
        theLookForwardSkill();
        theWalkAtRelativeSpeedSkill(Pose2f(walkSpeed, 0.f, 0.f));
      }
    }
/*======================================================================================================*/
    state(CornerKickToOwnRobot)
    {
      transition
      {
       
        if(theGameInfo.setPlay == SET_PLAY_NONE)
        {
          goto goToBallAndKick;
        }
        else if(theGameInfo.setPlay == SET_PLAY_PUSHING_FREE_KICK)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto PushingFreeKick;
          }
          else
          {
            goto BuildWall;
          }
        }
        else if(theGameInfo.setPlay == SET_PLAY_GOAL_KICK)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto GoalFreeKick;
          }
          else
          {
            goto BuildWall;
          }
        }
        else if(theGameInfo.setPlay == SET_PLAY_KICK_IN)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto KickIn;
          }
          else
          {
            goto BuildWall;
          }
        }
      }

      action
      {
        if(theRobotInfo.number == 3)
        {
          theGoToBallAndKickSkill(calcAngleToGoal(), KickInfo::walkForwardsLeftLong);
        }
        else
        {
          if(theRobotInfo.number == 2)
            theWalkToKickoffPoseSkill(Pose2f(0.f, 1.6*R4X, R2Y));
          else if(theRobotInfo.number == 4)
            theWalkToKickoffPoseSkill(Pose2f(0.f, 1.6*R4X, R4Y));

          theLookActiveSkill(/* withBall: */ true, /* ignoreBall: */ false);
          //theStandSkill(/* high: */ true);
        }
      }
    }
/*======================================================================================================*/
    state(KickIn)
    {
      transition
      {
        
        if(theGameInfo.setPlay == SET_PLAY_NONE)
        {
          goto goToBallAndKick;
        }
        else if(theGameInfo.setPlay == SET_PLAY_PUSHING_FREE_KICK)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto PushingFreeKick;
          }
          else
          {
            goto BuildWall;
          }
        }
        else if(theGameInfo.setPlay == SET_PLAY_GOAL_KICK)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto GoalFreeKick;
          }
          else
          {
            goto BuildWall;
          }
        }
        else if(theGameInfo.setPlay == SET_PLAY_CORNER_KICK)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto CornerKickToOwnRobot;
          }
          else
          {
            goto BuildWall;
          }
        }
      }

      action
      {
        if(theRobotInfo.number == 3)
        {
          theGoToBallAndKickSkill(calcAngleToGoal(), KickInfo::walkForwardsLeftLong);
        }
        else
        {
          if(theRobotInfo.number == 2)
            theWalkToKickoffPoseSkill(Pose2f(0.f, 1.6*R4X, R2Y));
          else if(theRobotInfo.number == 4)
            theWalkToKickoffPoseSkill(Pose2f(0.f, 1.6*R4X, R4Y));
          
          theLookActiveSkill(/* withBall: */ true, /* ignoreBall: */ false);
          //theStandSkill(/* high: */ true);
        }
      }
    }
/*======================================================================================================*/
    state(GoalFreeKick)
    {
      transition
      {
        
        if(theGameInfo.setPlay == SET_PLAY_NONE)
        {
          goto goToBallAndKick;
        }
        else if(theGameInfo.setPlay == SET_PLAY_PUSHING_FREE_KICK)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto PushingFreeKick;
          }
          else
          {
            goto BuildWall;
          }
        }
        else if(theGameInfo.setPlay == SET_PLAY_KICK_IN)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto KickIn;
          }
          else
          {
            goto BuildWall;
          }
        }
        else if(theGameInfo.setPlay == SET_PLAY_CORNER_KICK)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto CornerKickToOwnRobot;
          }
          else
          {
            goto BuildWall;
          }
        }
      }

      action
      {
        if(theRobotInfo.number == 3)
        {
          theGoToBallAndKickSkill(calcAngleToGoal(), KickInfo::walkForwardsLeftLong);
        }
        else
        {
          if(theRobotInfo.number == 2)
            theWalkToKickoffPoseSkill(Pose2f(0.f, 1.6*R4X, R2Y));
          else if(theRobotInfo.number == 4)
            theWalkToKickoffPoseSkill(Pose2f(0.f, 1.6*R4X, R4Y));
          
          theLookActiveSkill(/* withBall: */ true, /* ignoreBall: */ false);
          //theStandSkill(/* high: */ true);
        }
      }
    }
/*======================================================================================================*/
    state(PushingFreeKick)
    {
      transition
      {
        
        if(theGameInfo.setPlay == SET_PLAY_NONE)
        {
          goto goToBallAndKick;
        }
        else if(theGameInfo.setPlay == SET_PLAY_KICK_IN)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto KickIn;
          }
          else
          {
            goto BuildWall;
          }
        }
        else if(theGameInfo.setPlay == SET_PLAY_CORNER_KICK)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto CornerKickToOwnRobot;
          }
          else
          {
            goto BuildWall;
          }
        }
        else if(theGameInfo.setPlay == SET_PLAY_GOAL_KICK)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto GoalFreeKick;
          }
          else
          {
            goto BuildWall;
          }
        }
      }

      action
      {
        if(theRobotInfo.number == 3)
        {
          theGoToBallAndKickSkill(calcAngleToGoal(), KickInfo::walkForwardsLeftLong);
        }
        else
        {
          if(theRobotInfo.number == 2)
            theWalkToKickoffPoseSkill(Pose2f(0.f, 1.6*R4X, R2Y));
          else if(theRobotInfo.number == 4)
            theWalkToKickoffPoseSkill(Pose2f(0.f, 1.6*R4X, R4Y));

          theLookActiveSkill(/* withBall: */ true, /* ignoreBall: */ false);
          //theStandSkill(/* high: */ true);
        }
      }
    }
/*======================================================================================================*/
    state(BuildWall)
    {
      transition
      {
        
        if(theGameInfo.setPlay == SET_PLAY_NONE)
        {
          goto goToBallAndKick;
        }
        else if(theGameInfo.setPlay == SET_PLAY_PUSHING_FREE_KICK)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto PushingFreeKick;
          }
          else
          {
            goto BuildWall;
          }
        }
        else if(theGameInfo.setPlay == SET_PLAY_KICK_IN)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto KickIn;
          }
          else
          {
            goto BuildWall;
          }
        }
        else if(theGameInfo.setPlay == SET_PLAY_CORNER_KICK)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto CornerKickToOwnRobot;
          }
          else
          {
            goto BuildWall;
          }
        }
        else if(theGameInfo.setPlay == SET_PLAY_GOAL_KICK)
        {
          if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
          {
            goto GoalFreeKick;
          }
          else
          {
            goto BuildWall;
          }
        }
      }

      action
      {
          if(theRobotInfo.number == 2)
            theWalkToKickoffPoseSkill(Pose2f(0.f, 1.6 * R4X, R2Y));
          else if(theRobotInfo.number == 3)
            theWalkToKickoffPoseSkill(Pose2f(0.f, 1.6 * R4X, R3Y));
          else if(theRobotInfo.number == 4)
            theWalkToKickoffPoseSkill(Pose2f(0.f, 1.6 * R4X, R4Y));

          theLookActiveSkill(/* withBall: */ true, /* ignoreBall: */ false);
          //theStandSkill(/* high: */ true);
      }
    }
    state(goalKeeper)
    {
      action
      {
        theWalkToKickoffPoseSkill(Pose2f(0.f, R1X + (theFieldDimensions.xPosOwnGoalArea - R1X)/2, R1Y));
        theLookActiveSkill(/* withBall: */ true, /* ignoreBall: */ false);

        //if(theFieldBall.recentBallPositionOnField()[0] <  theFieldDimensions.xPosOwnGoalArea)
        if((theFieldBall.recentBallPositionOnField()[0] <  theFieldDimensions.xPosOwnPenaltyMark) && (theFieldBall.isRollingTowardsOwnGoal == true))
        {
          //theInterceptBallSkill(Interception::jumpLeft);
          
          //theInterceptBallSkill(Interception::walk);

          //theInterceptBallSkill(Interception::genuflectStand);
          // genuflectFromSitting
          
          if((theFieldBall.recentBallPositionOnField()[1] <  theFieldDimensions.yPosLeftGoal) && (theFieldBall.recentBallPositionOnField()[1] > 0.2 * theFieldDimensions.yPosLeftGoal))
          {
            theInterceptBallSkill(Interception::jumpLeft);
          }
          else if((theFieldBall.recentBallPositionOnField()[1] <  0.2 * theFieldDimensions.yPosLeftGoal) && (theFieldBall.recentBallPositionOnField()[1] > 0.2 * theFieldDimensions.yPosRightGoal))
          {
            theInterceptBallSkill(Interception::walk);
            // theInterceptBallSkill(Interception::genuflectStandDefender); //genuflectFromSitting
            
          }
          else if((theFieldBall.recentBallPositionOnField()[1] <  0.2 * theFieldDimensions.yPosRightGoal) && (theFieldBall.recentBallPositionOnField()[1] > theFieldDimensions.yPosRightGoal))
          {
            theInterceptBallSkill(Interception::jumpRight); 
          }

        }
        #if 0
        else if(theFieldBall.recentBallPositionOnField()[0] <  theFieldDimensions.xPosOwnGoalArea)
        {
          theInterceptBallSkill(Interception::genuflectStand);
          //theWalkToKickoffPoseSkill(Pose2f(0.f, R1X, R1Y));
          //theLookActiveSkill(/* withBall: */ true, /* ignoreBall: */ false);
        }
        #endif

        // if(theFieldBall.recentBallPositionOnField()[0] < 0.6 * theFieldDimensions.xPosOwnGoal)
        // {  
        //   theInterceptBallSkill(Interception::jumpLeft);
        // }
      }
      transition
      {
        //if((theFieldBall.recentBallPositionOnField()[0] < theFieldDimensions.xPosOwnGoalArea + 800) && (theFieldBall.recentBallPositionOnField()[0] > theFieldDimensions.xPosOwnGoalArea))
        if((isInsideOwnPenaltyMarkArea() == true) && (theFieldBall.isRollingTowardsOwnGoal == false)) 
        
        {
          goto noGoalKeep;
        } 
        //theAfterInterceptBallSkill();
      }
    }

    state(noGoalKeep)
    {
      action 
      {
        //theLookActiveSkill(/* withBall: */ true, /* ignoreBall: */ false);
        theGoToBallAndKickSkill(calcAngleToGoal(), KickInfo::walkForwardsLeftLong);
      }
      transition
      {
        //if(theFieldBall.recentBallPositionOnField()[0] > theFieldDimensions.xPosOwnGoalArea)
        // if(theFieldBall.isRollingTowardsOwnGoal == true)
        if(isInsideOwnPenaltyMarkArea() == false)
        {
          goto goalKeeper;
        }
      }
    }

    /* 
    state(supporter2)
    {
      transition
      {
      
      }

      action
      {
        theWalkToKickoffPoseSkill(Pose2f(0.f, theFieldBall.recentBallPositionOnField()[0], R2Y));
      }
    }

    state(supporter3)
    {
      transition
      {
      
      }

      action
      {
        theWalkToKickoffPoseSkill(Pose2f(0.f, theFieldBall.recentBallPositionOnField()[0], R3Y));
      }
    }
    */
  }
  Angle calcAngleToGoal() const
  {
    if(abs(theFieldBall.recentBallPositionOnField()[1]) < 0.9 * theFieldDimensions.yPosLeftGoal && theFieldBall.recentBallPositionOnField()[0] > 0.8 * theFieldDimensions.xPosOpponentGoal)
    {
      return (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundLine, theFieldBall.recentBallPositionOnField()[1])).angle();
    }
    
    return (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundLine, 0.f)).angle();
  }

  bool isInsideOwnPenaltyMarkArea() const
  {
    if(theFieldBall.recentBallPositionOnField()[0] < theFieldDimensions.xPosOwnPenaltyMark) 
    {
      if(abs(theFieldBall.recentBallPositionOnField()[1]) <  theFieldDimensions.yPosLeftGoalArea) //yPosLeftGoal
        return true;
    }
    return false;    
  }
  
  //bool SelectionBetweenDribbleAndKick() const
  //{
  //  return Pose2f(theFieldBall.recentBallPositionOnField()) - Pose2f xPosHalfWayLine + theFieldDimensions.xPosOpponentGoalPost)/2;
  //}
};

MAKE_CARD(CodeReleaseKickAtGoalCard);
