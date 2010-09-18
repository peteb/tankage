/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "body.h"

Physics::Body::Body(Physics::Subsystem & subsystem, rect * area)
   : subsystem(subsystem)
   , area(area)
{
   
}