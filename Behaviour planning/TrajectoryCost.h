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


class TrajectoryCost{

    

public:
    
private:
    
    struct TrajectoryData{
        int proposedLane;
        int avgSpeed;
        int maxAccl;
        float rmsAccl;
        int closestDist;
        int lastDistToGoal;
        int lastLaneDiffFromGoal;
        bool collides;
    };
    
    
    typedef double (*CostFn)(Vehicle,vector<Vehicle::SnapShot>, map<int,vector< vector<int> > >,TrajectoryCost::TrajectoryData ) ;
    CostFn change_lane_cost, distance_from_goal_lane, speed_cost, collision_cost, buffer_cost;
    CostFn cf[5] = {change_lane_cost, distance_from_goal_lane, speed_cost, collision_cost, buffer_cost};
    
    
};


#endif /* TrajectoryCost_hpp */
