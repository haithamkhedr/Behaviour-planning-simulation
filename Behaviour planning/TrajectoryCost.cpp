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
    int proposed_lane =  trajectoryData.proposedLane;
    double cost  = 0;
    
    if(proposed_lane > currLane)
        cost = COMFORT;
    else if(proposed_lane < currLane)
        cost = -COMFORT;
    
    return cost;
}

double TrajectoryCost::distance_from_goal_lane(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions,TrajectoryCost::TrajectoryData trajectoryData){
    
    int dd = trajectoryData.lastLaneDiffFromGoal;
    int ds = trajectoryData.lastDistToGoal;
    double timeToGoal = ds / trajectoryData.avgSpeed;
    double multiplier = (5 * dd) /timeToGoal;
    double cost = multiplier * REACH_GOAL;
    
    return cost;
}

double TrajectoryCost::speed_cost(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions,TrajectoryCost::TrajectoryData trajectoryData){
    int target_speed = vehicle.target_speed;
    float avgSpeed = trajectoryData.avgSpeed;
    double multiplier = (target_speed - avgSpeed) / target_speed;
    double cost = multiplier * multiplier * EFFICIENCY;
    return cost;
}


double TrajectoryCost::collision_cost(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions,TrajectoryCost::TrajectoryData trajectoryData){
    double cost = 0;
    if(trajectoryData.collides.collision == true) {
        double timeToCollision = trajectoryData.collides.time;
        timeToCollision *= timeToCollision;
        double multiplier = exp(-timeToCollision);
        cost = multiplier * COLLISION;
    }
    return cost;
}


double TrajectoryCost::buffer_cost(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions,TrajectoryCost::TrajectoryData trajectoryData){
    
    int closestDist = trajectoryData.closestDist;
    float tToCollision = closestDist / (float) trajectoryData.avgSpeed;
    double cost = 0;
    if(closestDist == 0)
        cost = 10 * DANGER;
    
    else if(tToCollision > DESIRED_BUFFER){
        cost = 0;
    }
    else{
        double multiplier = tToCollision/DESIRED_BUFFER;
        multiplier *= multiplier;
        multiplier = 1 - multiplier;
        cost = multiplier * DANGER;
        
    }
    
    return cost;
}

map<int,vector< vector<int> > > TrajectoryCost::filterVehicles(map<int,vector< vector<int> > > predictions, int lane){
    map<int,vector< vector<int> > > ret;
    map<int,vector< vector<int> > >::iterator it;
    for(it = predictions.begin(); it != predictions.end(); it++){
        if(it->second[0][0] == lane && it->first != -1 ){
            ret[it->first] = it->second;
        }
    }
    return ret;
}

TrajectoryCost::TrajectoryData TrajectoryCost::getTrajectoryData(const vector<Vehicle::SnapShot>& trajectory, const Vehicle& vehicle, map<int,vector< vector<int> > > predictions){
    
    TrajectoryCost::TrajectoryData trData;
    Vehicle::SnapShot last = trajectory[trajectory.size()-1];
    Vehicle::SnapShot first = trajectory[1];
    Vehicle::SnapShot current = trajectory[0];
    int dt =  (trajectory.size());
    
    if(first.state == "PLCR"){
        trData.proposedLane = min(current.lane-1, vehicle.lanes_available);
    }
    else if(first.state == "PLCL"){
        trData.proposedLane = max(0,current.lane+1);
        
    }
    trData.avgSpeed = (last.s - current.s) / (float)dt;
    trData.lastDistToGoal = vehicle.goal_s - last.s;
    trData.lastLaneDiffFromGoal = abs(last.lane - vehicle.goal_lane);
    
    vector<int> accels;
    Collider collider;
    collider.collision = false;
    int closestDist = 9999999;
    int maxAcc = -99999;
    int rmsAcc = 0;
    map<int,vector< vector<int> > > laneVehicles = filterVehicles(predictions,last.lane);
    for(int i = 1; i < dt; ++i){
        Vehicle::SnapShot myCar = trajectory[i];
        int a = myCar.a;
        accels.push_back(a);
        rmsAcc += (a*a);
        if(a >maxAcc)
            maxAcc = a;
        
        for(map<int,vector< vector<int> > >::iterator j= laneVehicles.begin() ; j != laneVehicles.end() ; j++){
            
            vector<int> otherCar_prev = j->second[i-1];
            vector<int> otherCar_state = j->second[i];
            if(collider.collision == false){
                if( (otherCar_prev[1] < myCar.s && otherCar_state[1] >= myCar.s) ||
                   (otherCar_prev[1] > myCar.s && otherCar_state[1] <= myCar.s)) {
                    collider.collision = true;
                    collider.time = i;
                }
                
            }
            int distance = abs(otherCar_state[1] - myCar.s);
            if(distance < closestDist){
                closestDist = distance;
            }
            
            
            
        }
        
    }
    
    trData.maxAccl = maxAcc;
    trData.rmsAccl = (float)rmsAcc / accels.size();
    trData.closestDist = closestDist;
    trData.collides = collider;
    
    
    return trData;
}





double TrajectoryCost::calculateCost(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions){
    
    double cost = 0;
    TrajectoryCost::TrajectoryData trajData =  getTrajectoryData(trajectory,vehicle,predictions);
    for(int i = 0; i< cf.size(); i++){
        CostFnPtr costFunc = cf[i];
        cost += (this->*costFunc)(vehicle, trajectory, predictions,trajData);
    }
    return cost;
}
