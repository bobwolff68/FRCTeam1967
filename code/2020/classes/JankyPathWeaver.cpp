#include <iostream>
#include <frc/Filesystem.h>
#include <wpi/Path.h>
#include <wpi/SmallString.h>
#include <frc/trajectory/TrajectoryConfig.h>
#include <frc/trajectory/TrajectoryUtil.h>
#include <frc/trajectory/TrajectoryGenerator.h>
#include <frc/trajectory/constraint/DifferentialDriveVoltageConstraint.h>
#include "frc/kinematics/DifferentialDriveKinematics.h"
#include <frc/kinematics/DifferentialDriveWheelSpeeds.h>
#include "frc2/command/RamseteCommand.h"
// our header files
#include "AutoConstants.h"
#include "AutoDriveSubsystems.h"
#include "AutoSettings.h"
#include "AutoDriveSubsystems.h"
#include "JankyPathWeaver.h"
#include "AutoEntry.h"

using namespace AutoDriveConstants;
using namespace frc;
using namespace frc2;

JankyPathWeaver::JankyPathWeaver(AutoDriveSubsystem * drive, int autoMode) 
{
    // create auto drive subsystem
    m_drive = drive;
    
    leftController.SetTolerance(0.0);
    rightController.SetTolerance(0.0);

    // generate path
    GeneratePath(autoMode);

    // initialize ramsete command
    //rc = NULL;
    //SetUpRamseteCommand();
}

JankyPathWeaver::~JankyPathWeaver() {
    //delete rc;
    delete m_drive;
}

void JankyPathWeaver::GeneratePath(int autoMode) {
    // Path name variable
    std::string pathName = "GetBallFromTrench.wpilib.json";

    // Set up config for trajectory
    frc::TrajectoryConfig config(AutoConstants::kMaxSpeed, AutoConstants::kMaxAcceleration);
    // Add kinematics to ensure max speed is actually obeyed
    config.SetKinematics(AutoDriveConstants::kDriveKinematics);
    // Apply the voltage constraint
    config.AddConstraint(autoVoltageConstraint);

    // Assign path name
    wpi::SmallString<64> deployDirectory;
    frc::filesystem::GetDeployDirectory(deployDirectory);
    wpi::sys::path::append(deployDirectory, "paths");
    if(autoMode == STAY_STILL)
    {
        pathName == "StayStill.wpilib.json";
    }
    else if(autoMode == FORWARD_STILL) {
        pathName == "ForwardStill.wpilib.json";
    }
    else if(autoMode == BACKWARD_STILL) {
        pathName == "BackwardStill.wpilib.json";
    }
    else if(autoMode == CENTER_SHOOT) {
        pathName == "CenterShoot.wpilib.json";
    }
    else if(autoMode == CENTER_FORWARD_SHOOT) {
        pathName == "CenterForwardShoot.wpilib.json";
    }
    else if(autoMode == CENTER_BACKWARD_SHOOT) {
        pathName == "CenterBackwardShoot.wpilib.json";
    }
    else if(autoMode == CENTER_SHOOT_RONDEZVOUS) {
        pathName == "CenterShootRondezvous.wpilib.json";
    }
    else if(autoMode == CENTER_SHOOT_TRENCH) {
        pathName == "CenterShootTrench.wpilib.json";
    }
    else if(autoMode == CENTER_FORWARD_RONDEZVOUS) {
        pathName == "ForwardRondezvous.wpilib.json";
    }
    else if(autoMode == LEFT_FORWARD_RONDEZVOUS) {
        pathName == "ForwardRondezvous.wpilib.json";
    }
    else if(autoMode == RIGHT_FORWARD_RONDEZVOUS) {
        pathName == "ForwardRondezvous.wpilib.json";
    }
    else if(autoMode == LC_FORWARD_RONDEZVOUS) {
        pathName == "ForwardRondezvous.wpilib.json";
    }
    else if(autoMode == RC_FORWARD_RONDEZVOUS) {
        pathName == "ForwardRondezvous.wpilib.json";
    }
    else if(autoMode == CENTER_BACKWARD_RONDEZVOUS) {
        pathName == "BackwardRondezvous.wpilib.json";
    }
    else if(autoMode == LEFT_BACKWARD_RONDEZVOUS) {
        pathName == "BackwardRondezvous.wpilib.json";
    }
    else if(autoMode == RIGHT_BACKWARD_RONDEZVOUS) {
        pathName == "BackwardRondezvous.wpilib.json";
    }
    else if(autoMode == LC_BACKWARD_RONDEZVOUS) {
        pathName == "BackwardRondezvous.wpilib.json";
    }
    else if(autoMode == RC_BACKWARD_RONDEZVOUS) {
        pathName == "BackwardRondezvous.wpilib.json";
    }
    else if(autoMode == CENTER_FORWARD_TRENCH) {
        pathName == "ForwardTrench.wpilib.json";
    }
    else if(autoMode == LEFT_FORWARD_TRENCH) {
        pathName == "ForwardTrench.wpilib.json";
    }
    else if(autoMode == RIGHT_FORWARD_TRENCH) {
        pathName == "ForwardTrench.wpilib.json";
    }
    else if(autoMode == RC_FORWARD_TRENCH) {
        pathName == "ForwardTrench.wpilib.json";
    }
    else if(autoMode == LC_FORWARD_TRENCH) {
        pathName == "ForwardTrench.wpilib.json";
    }
    else if(autoMode == CENTER_BACKWARD_TRENCH) {
        pathName == "BackwardTrench.wpilib.json";
    }
    else if(autoMode == RIGHT_BACKWARD_TRENCH) {
        pathName == "BackwardTrench.wpilib.json";
    }
    else if(autoMode == LEFT_BACKWARD_TRENCH) {
        pathName == "BackwardTrench.wpilib.json";
    }
    else if(autoMode == LC_BACKWARD_TRENCH) {
        pathName == "BackwardTrench.wpilib.json";
    }
    else if(autoMode == RC_BACKWARD_TRENCH) {
        pathName == "BackwardTrench.wpilib.json";
    }
    else if(autoMode == CENTER_BACKWARD_SHOOT_RONDEZVOUS) {
        pathName == "BackwardShootRondezvous.wpilib.json";
    }
    else if(autoMode == RIGHT_BACKWARD_SHOOT_RONDEZVOUS) {
        pathName == "BackwardShootRondezvous.wpilib.json";
    }
    else if(autoMode == RC_BACKWARD_SHOOT_RONDEZVOUS) {
        pathName == "BackwardShootRondezvous.wpilib.json";
    }
    else if(autoMode == CENTER_BACKWARD_SHOOT_TRENCH) {
        pathName == "BackwardShootTrench.wpilib.json";
    }
    else if(autoMode == RIGHT_BACKWARD_SHOOT_TRENCH) {
        pathName == "BackwardShootTrench.wpilib.json";
    }
    else if(autoMode == RC_BACKWARD_SHOOT_TRENCH) {
        pathName == "BackwardShootTrench.wpilib.json";
    }

    wpi::sys::path::append(deployDirectory, pathName);

    // get trajectory
    trajectory = frc::TrajectoryUtil::FromPathweaverJson(deployDirectory);
}

// void JankyPathWeaver::SetUpRamseteCommand() {
//     rc = new frc2::RamseteCommand(
//       trajectory, [this]() { return m_drive->GetPose(); },
//       frc::RamseteController(AutoConstants::kRamseteB,
//                             AutoConstants::kRamseteZeta),
//       frc::SimpleMotorFeedforward<units::meters>(
//           AutoDriveConstants::ks, AutoDriveConstants::kv, AutoDriveConstants::ka),
//       AutoDriveConstants::kDriveKinematics,
//       [this] { return m_drive->GetWheelSpeeds(); },
//       leftController,
//       rightController,
//       [this](auto left, auto right) { m_drive->TankDriveVolts(left, right); },
//       {&m_drive});

//       rc->Initialize();
// }

void JankyPathWeaver::Start()
{
    // rc->Execute();
}

bool JankyPathWeaver::JobDone()
{
    return true; // (rc->IsFinished());
}

void JankyPathWeaver::RunAction()
{

}

void JankyPathWeaver::End()
{

}