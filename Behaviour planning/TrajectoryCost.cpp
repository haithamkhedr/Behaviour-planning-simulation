//
//  TrajectoryCost.cpp
//  Behaviour planning
//
//  Created by Haitham Khedr on 7/31/17.
//  Copyright © 2017 Haitham Khedr. All rights reserved.
//

#include "TrajectoryCost.h"
#include "vehicle.h"



double TrajectoryCost::change_lane_cost(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions,TrajectoryCost::TrajectoryData trajectoryData)


{
    int currLane = vehicle.lane;
    int proposed_lane =  trajectoryData.lastLaneDiffFromGoal; 	
    double cost  = 0;

    if(proposed_lane > currLane)
	cost = COMFORT;
    else if(proposed_lane < currLane)
	cost = -COMFORT;

    return cost;
}

double TrajectoryCost::distance_from_goal_lane(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions,TrajectoryCost::TrajectoryData trajectoryData){

    int dd = trajectoryData.lastLaneDiffFromGoal
    int ds = trajectoryData.lastDistToGoal;
    double timeToGoal = ds / trajectoryData.avgSpeed;
    double multiplier = (5 * dd) /timeToGoal;
    double cost = multiplier * REACH_GOAL;

    return cost;
}

double TrajectoryCost::speed_cost(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions,TrajectoryCost::TrajectoryData trajectoryData){
    int target_speed = vehicle.target_speed;
    int avgSpeed = trajectoryData.avgSpeed;
    double multiplier = (target_speed - avgSpeed) / target_speed;
    double cost = multiplier * multiplier * EFFICIENCY;
    return cost;
}


double TrajectoryCost::collision_cost(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions,TrajectoryCost::TrajectoryData trajectoryData){
    double cost = 0
    if(trajectoryData.collides.collision == true) {
	double timeToCollision = trajectoryData.collides.time;	
    	timeToCollision *= timeToCollision
    	double multiplier = exp(-timeToCollision)
    	double cost = multiplier * COLLSION;
    }
    return cost;
}


double TrajectoryCost::buffer_cost(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions,TrajectoryCost::TrajectoryData trajectoryData){

    int closestDist = trajectoryData.closestDist;
    float tToCollision = closestDist / (float) trajectoryData.avgSpeed;
    double cost = 0;
    if(tToCollision == 0)
	cost = 10 * DANGER;
    else if(tToCollision > DESIRED_BUFFER){
	cost = 0;
    }
    else{
	double multiplier = tToCollision/DESIRED_BUFFER;
        multiplier* = multiplier;
	multiplier = 1 - multiplier;
	cost = multiplier * DANGER;
    	
    }

    return cost;
}


TrajectoryCost::TrajectoryData getTrajectoryData(const vector<Vehicle::SnapShot>& trajectory, const Vehicle& vehicle, map<int,vector< vector<int> > > predictions){

    TrajectoryCost::TrajectoryData trData;
    Vehicle::SnapShot last = trajectory[trajectory.size()-1];
    Vehicle::SnapShot first = trajectory[1];
    Vehicle::SnapShot current = trajectory[0];
    int dt =  (trajectory.size());

    trData.proposedLane = last.lane;
    trData.avgSpeed = (last.v - current.v) / dt;
    trData.lastDistToGoal = vehicle.goal_s - last.s;
    trData.lastLaneDiffFromGoal = abs(last.lane - vehicle.goal_lane);

    vector<int> accels;
    TrajectoryCost::Collider collider;
    collider.collides = false;
    int closestDist = 9999999;
    int maxAcc = -99999;
    int rmsAcc = 0;

    laneVehicles = filterVehicles(map<int,vector< vector<int> > > predictions, int lane);
    for(int i = 1; i < dt; ++i){
	Vehicle::SnapShot myCar = trajectory[i]; 
	int a = myCar.a;
	accels.push_back(a);
	rmsAcc += (a*a);
	if(a >maxAcc)
	    maxAcc = a;

	for(int j=0;j < laneVehicles.size(); j++){
	    	
	    vector<int> otherCar_curr = laneVehicles[j][i-1];
	    vector<int> otherCar_state = laneVehicles[j][i];
	    if(collider.collides == false){
		if( (otherCar_curr[0] < myCar.s && otherCar_state[0] >= myCar.s) ||
			(otherCar_curr[0] > myCar.s && otherCar_state[0] <= myCar.s)) {
		    collider.collides = true;
		    collider.time = i;
		}
	    
	    }
	    int distance = abs(otherCar_state[0] - myCar.s);
	    if(distance < closestDist){
		closestDist = distance;
	    }



	}

    }

    trData.maxAccl = maxAcc;
    trData.rmsAccl = (float)rmsAcc / accels.size();
    trData.closestDist = closestDist;
    trData.collides = collider;


    return TrajectoryCost::TrajectoryData();
}

map<int,vector< vector<int> > > filterVehicles(map<int,vector< vector<int> > > predictions, int lane){
    map<int,vector< vector<int> > > ret;
    map<int,vector< vector<int> > >::iterator it;
    for(it = predictions.begin(); it != predictions.end(); it++){
	if(it->second[0][1] == lane && it->first != -1 ){
	    ret[it->first] = it->second;
    }
	return ret;

}



double TrajectoryCost::calculateCost(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions){

    double cost = 0;
    TrajectoryCost::TrajectoryData trajData =  getTrajectoryData(trajectory,predictions);
    for(int i = 0; i< cf.size(); i++){
	cost += cf[0](vehicle, trajectory, predictions,trajData);
    }
    return cost;
}
