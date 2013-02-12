/* 
 * File:   Position.h
 * Author: radek
 *
 * Created on 24 listopad 2012, 18:39
 */

#ifndef POSITION_H
#define	POSITION_H

class Position{
    int solarSystem,planet,side;
    short x,y;
    
};

struct EntityPosition{
    float velocity[3],rotation[3],position[3];
};


#endif	/* POSITION_H */

