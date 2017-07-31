//
//  TrajectoryCost.cpp
//  Behaviour planning
//
//  Created by Haitham Khedr on 7/31/17.
//  Copyright © 2017 Haitham Khedr. All rights reserved.
//

#include "TrajectoryCost.h"



double TrajectoryCost::change_lane_cost(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions,TrajectoryCost::TrajectoryData trajectoryData){
    return 0;
}
double TrajectoryCost::distance_from_goal_lane(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions,TrajectoryCost::TrajectoryData trajectoryData){
    return 0;
}
double TrajectoryCost::speed_cost(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions,TrajectoryCost::TrajectoryData trajectoryData){
    return 0;
}
double TrajectoryCost::collision_cost(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions,TrajectoryCost::TrajectoryData trajectoryData){
    return 0;
}
double TrajectoryCost::buffer_cost(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions,TrajectoryCost::TrajectoryData trajectoryData){
    return 0;
}


TrajectoryCost::TrajectoryData getTrajectoryData(const vector<Vehicle::SnapShot>& trajectory, const Vehicle& vehicle, map<int,vector< vector<int> > > predictions){
    return TrajectoryCost::TrajectoryData();
}

