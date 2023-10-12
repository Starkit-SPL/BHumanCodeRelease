/**
 * @file CodeReleasePositionForKickOffCard.cpp
 *
 * This file implements nothing.
 *
 * @author Arne Hasselbring
 */

/* old coordinates

#define R1X -4500
#define R1Y 0

#define R3X -2000
#define R3Y 1200 // -1200

#define R2X -1200
#define R2Y 0

#define R4X -2000
#define R4Y -1200

*/



#include "Representations/BehaviorControl/Skills.h"
#include "Tools/BehaviorControl/Framework/Card/Card.h"

#include "Representations/Communication/RobotInfo.h"
#include "Representations/Communication/GameInfo.h"
#include "Representations/Communication/TeamInfo.h"

// #include "Representations/BehaviorControl/Skills.h"
#include "Representations/Modeling/RobotPose.h"
// #include "Tools/BehaviorControl/Framework/Card/Card.h"
#include "Tools/BehaviorControl/Framework/Card/CabslCard.h"
#include "Representations/BehaviorControl/FieldBall.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Tools/Math/BHMath.h"


#include "Representations/BehaviorControl/TeamBehaviorStatus.h"

#include "Representations/Configuration/BallSpecification.h"

#include "Modules/Modeling/BallInGoalTracker/BallInGoalTracker.h"

// --- for_brazil ----
// #define R1X -4500
// #define R1Y 0

#define R1X theFieldDimensions.xPosOwnGroundLine
#define R1Y 0

#define R3X -1300 
#define R3Y 700 // 1200

#define R4X -700
#define R4Y 0

#define R2X -1300
#define R2Y -700 //-1200
 
// #define R5X -3500
// #define R5Y -1200


CARD(CodeReleasePositionForKickOffCard,
{,
  CALLS(Activity),
  CALLS(LookForward),
  CALLS(Say),
  CALLS(Stand),

  CALLS(LookActive),
  CALLS(LookAtPoint),

  CALLS(WalkToKickoffPose),
  CALLS(PenaltyStrikerGoToBallAndKick),
  CALLS(GoToBallAndKick),
  CALLS(WalkAtRelativeSpeed),

  CALLS(InterceptBall),


  
  REQUIRES(RobotInfo),
  REQUIRES(GameInfo),

  REQUIRES(OwnTeamInfo),
  REQUIRES(OpponentTeamInfo),
  REQUIRES(FieldBall),
  REQUIRES(FieldDimensions),

  REQUIRES(BallSpecification),
  REQUIRES(RobotPose),
  REQUIRES(TeamBehaviorStatus),

  DEFINES_PARAMETERS(
  {,
    // (float)(0.8f) walkSpeed,
    (int)(1000) initialWaitTime,
    // (int)(7000) ballNotSeenTimeout,
  }),

});

class CodeReleasePositionForKickOffCard : public CodeReleasePositionForKickOffCardBase
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
    theActivitySkill(BehaviorStatus::codeReleasePositionForKickOff);
    
    initial_state(start)
    {
      action
      {
        theLookForwardSkill();
        theStandSkill();
      }

      transition
      {

        if(state_time > initialWaitTime)
          goto stateCondition; 

      }

    }

    state(stateCondition)
    {
      action
      {

      }

      transition
      {
        if(theGameInfo.state == STATE_READY)
        {
          if(theGameInfo.setPlay == SET_PLAY_PENALTY_KICK)
          {
            if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber) 
              goto ownPenaltyKick;
            else
              goto opponentPenaltyKick;
          }
          else
          {
            if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
              goto ownKickoff;
            else
              goto opponentKickoff;
          }
        }

        else //STATE_PLAYING
        {
          if(theGameInfo.setPlay != SET_PLAY_NONE)
          {
            if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
            {
              if(theGameInfo.setPlay == SET_PLAY_PENALTY_KICK)
                goto ownPenaltyKick;
            }
            else
            {
              if(theGameInfo.setPlay == SET_PLAY_PENALTY_KICK)
                goto opponentPenaltyKick;
            }
          }

        }
      }
    }

    state(ownKickoff)
    {
      action
      {

        if(theRobotInfo.number == 1) 
          theWalkToKickoffPoseSkill(Pose2f(0.f, R1X, R1Y));
        else if(theRobotInfo.number == 2)
          theWalkToKickoffPoseSkill(Pose2f(0.f, R2X, R2Y));
        else if(theRobotInfo.number == 3)
          theWalkToKickoffPoseSkill(Pose2f(0.f, R3X, R3Y));
        else if(theRobotInfo.number == 4)
          theWalkToKickoffPoseSkill(Pose2f(0.f, R4X + theFieldDimensions.centerCircleRadius/2, R4Y));
      }
      transition
      {
        
      }
    }
    state(opponentKickoff)
    {
      action
      {
        if(theRobotInfo.number == 1) 
          theWalkToKickoffPoseSkill(Pose2f(0.f, R1X, R1Y));
        else if(theRobotInfo.number == 2)
          theWalkToKickoffPoseSkill(Pose2f(0.f, R2X, R2Y));
        else if(theRobotInfo.number == 3)
          theWalkToKickoffPoseSkill(Pose2f(0.f, R3X, R3Y));
        else if(theRobotInfo.number == 4)
          theWalkToKickoffPoseSkill(Pose2f(0.f, R4X, R4Y));
      }
      transition
      {

      }
    }
    
    state(ownPenaltyKick)
    {
      action
      {
        // thePenaltyStrikerGoToBallAndKickSkill(Pose2f(theFieldDimensions.xPosPenaltyStrikerStartPosition, 0.f), KickInfo::walkForwardsLeftLong, 1);
        thePenaltyStrikerGoToBallAndKickSkill(Pose2f(theFieldDimensions.xPosPenaltyStrikerStartPosition, 0.f), KickInfo::walkForwardsLeftLong, 1.f);

        //SKILL_INTERFACE(PenaltyStrikerGoToBallAndKick, (const Pose2f&) kickPose, (KickInfo::KickType) kickType, (float) walkSpeed);

      }
      transition
      {

      }
    }

    state(opponentPenaltyKick)
    {
      action
      {
        theWalkToKickoffPoseSkill(Pose2f(0.f, R1X + (theFieldDimensions.xPosOwnGoalArea - R1X)/2, R1Y));
        theLookActiveSkill(/* withBall: */ true, /* ignoreBall: */ false);

        if((theFieldBall.recentBallPositionOnField()[0] <  theFieldDimensions.xPosOwnPenaltyMark - 300) && (theFieldBall.isRollingTowardsOwnGoal == true))
        {
          //theInterceptBallSkill(Interception::jumpLeft);
          
          //theInterceptBallSkill(Interception::walk);

          //theInterceptBallSkill(Interception::genuflectStand);
          
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
        //theGoToBallAndKickSkill(calcAngleToGoal(), KickInfo::walkForwardsLeftLong);
      }

      transition
      {

      }

    }

  }
  
  Angle calcAngleToGoal() const
  {
    return (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundLine, 0.f)).angle();
  }

/*
  void execute() override
  {
    theActivitySkill(BehaviorStatus::codeReleasePositionForKickOff);
    theLookForwardSkill();
    theStandSkill();

    if(theRobotInfo.number == 1) 
      theWalkToKickoffPoseSkill(Pose2f(0.f, R1X, R1Y));
    else if(theRobotInfo.number == 2)
      theWalkToKickoffPoseSkill(Pose2f(0.f, R2X, R2Y));
    else if(theRobotInfo.number == 3)
      theWalkToKickoffPoseSkill(Pose2f(0.f, R3X, R3Y));
    else if(theRobotInfo.number == 4)
      theWalkToKickoffPoseSkill(Pose2f(0.f, R4X, R4Y));


    // Not implemented in the Code Release.
    // theSaySkill("Please implement a behavior for me!");
    // theSaySkill("Starkit, where are you?");
    theSaySkill("Starkit, AAA!");

  }

*/
};


MAKE_CARD(CodeReleasePositionForKickOffCard);