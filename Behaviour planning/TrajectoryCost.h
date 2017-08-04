//
//  TrajectoryCost.hpp
//  Behaviour planning
//
//  Created by Haitham Khedr on 7/31/17.
//  Copyright © 2017 Haitham Khedr. All rights reserved.
//

#ifndef TrajectoryCost_hpp
#define TrajectoryCost_hpp

#include "vehicle.h"

#define COLLISION   10e6
#define DANGER      10e5
#define REACH_GOAL  10e5
#define COMFORT     10e4
#define EFFICIENCY  10e2
#define DESIRED_BUFFER 1.5

struct Collider{
    
    bool collision ; // is there a collision?
    int  time; // time collision happens
    
};

class TrajectoryCost{

    

public:

    double calculateCost(Vehicle vehicle,vector<Vehicle::SnapShot> trajectory, map<int,vector< vector<int> > > predictions);
    
    TrajectoryCost(){
        
        cf.push_back(&TrajectoryCost::change_lane_cost);
        cf.push_back(&TrajectoryCost::distance_from_goal_lane);
        cf.push_back(&TrajectoryCost::speed_cost);
        cf.push_back(&TrajectoryCost::collision_cost);
        cf.push_back(&TrajectoryCost::buffer_cost);
        
    }
    struct TrajectoryData{
        int proposedLane;
        float avgSpeed;
        int maxAccl;
        float rmsAccl;
        int closestDist;
        int lastDistToGoal;
        int lastLaneDiffFromGoal;
        Collider collides;
    };

private:
    
    
    
    typedef double (TrajectoryCost::*CostFnPtr)(Vehicle,vector<Vehicle::SnapShot>, map<int,vector< vector<int> > >,TrajectoryCost::TrajectoryData) ;
    typedef double (CostFn)(Vehicle,vector<Vehicle::SnapShot>, map<int,vector< vector<int> > >,TrajectoryCost::TrajectoryData ) ;

    CostFn change_lane_cost, distance_from_goal_lane, speed_cost, collision_cost, buffer_cost;
    vector<CostFnPtr> cf;
    map<int,vector< vector<int> > > filterVehicles(map<int,vector< vector<int> > > predictions, int lane);
    TrajectoryData getTrajectoryData(const vector<Vehicle::SnapShot>& trajectory, const Vehicle& vehicle, map<int,vector< vector<int> > > predictions);
   
    
};


#endif /* TrajectoryCost_hpp */
