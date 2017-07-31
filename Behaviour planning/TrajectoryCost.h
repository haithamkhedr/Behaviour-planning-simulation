//
//  TrajectoryCost.hpp
//  Behaviour planning
//
//  Created by Haitham Khedr on 7/31/17.
//  Copyright © 2017 Haitham Khedr. All rights reserved.
//

#ifndef TrajectoryCost_hpp
#define TrajectoryCost_hpp

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
        int lastLaneFromGoal;
        bool collides;
    };
    
    
};
#include <stdio.h>

#endif /* TrajectoryCost_hpp */
