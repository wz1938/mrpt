/* +---------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)               |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2016, Individual contributors, see AUTHORS file        |
   | See: http://www.mrpt.org/Authors - All rights reserved.                   |
   | Released under BSD License. See details in http://www.mrpt.org/License    |
   +---------------------------------------------------------------------------+ */

#include "nav-precomp.h" // Precomp header

#include <mrpt/nav/reactive/TWaypoint.h>
#include <limits>

using namespace mrpt::nav;
using namespace std;

const double INVALID_NUM = std::numeric_limits<double>::max();

// TWaypoint  ========== 
TWaypoint::TWaypoint() :
	target(INVALID_NUM,INVALID_NUM),
	target_heading(INVALID_NUM),
	allowed_distance(INVALID_NUM),
	allow_skip(true)
{
}

bool TWaypoint::isValid() const
{
	return 
	(target.x!=INVALID_NUM) && 
	(target.y!=INVALID_NUM) && 
	(allowed_distance!=INVALID_NUM);
}

std::string TWaypoint::getAsText() const
{
	std::string s;
	if (target.x!=INVALID_NUM && target.y!=INVALID_NUM)
		s+=mrpt::format("target=(%8.03f,%8.03f) ",target.x,target.y);
	else s+="target=(**Coordinates not set!!**) ";

	if (target_heading!=INVALID_NUM)
		s+=mrpt::format("phi=%8.03f deg ",mrpt::utils::RAD2DEG(target_heading));
	else s+=" (heading: any) ";

	if (allowed_distance!=INVALID_NUM)
		s+=mrpt::format("allowed_dist=%8.03f ",allowed_distance);
	else s+=" (**allowed_distance not set!!**) ";

	s+= (allow_skip ? " allow_skip: YES" : " allow_skip: NO ");

	return s;
}


// TWaypointSequence ========== 
TWaypointSequence::TWaypointSequence()
{
}

// Gets navigation params as a human-readable format:
std::string TWaypointSequence::getAsText() const 
{
	string s;
	s+=mrpt::format("List of %u waypoints:\n", static_cast<unsigned int>(waypoints.size()) );
	unsigned int i=0;
	for (auto &wp : waypoints) {
		s+= mrpt::format(" #%3u: ",i++);
		s+= wp.getAsText();
		s+= "\n";
	}
	return s;
}

// TWaypointStatus ========== 
TWaypointStatus::TWaypointStatus() :
	reached(false),
	timestamp_reach(INVALID_TIMESTAMP),
	skipped(false)
{
}
TWaypointStatus & TWaypointStatus::operator =(const TWaypoint &wp)
{
	TWaypoint::operator =(wp);
	return *this;
}
std::string TWaypointStatus::getAsText() const 
{
	std::string s = TWaypoint::getAsText();
	s += mrpt::format("reached=%s skipped=%s", (reached ? "YES":"NO "), (skipped ? "YES":"NO ") );;
	return s;
}

// TWaypointStatusSequence ======
TWaypointStatusSequence::TWaypointStatusSequence() : 
	waypoints(),
	timestamp_nav_started(INVALID_TIMESTAMP),
	final_goal_reached(false),
	waypoint_index_current_goal(-1)
{
}

std::string TWaypointStatusSequence::getAsText() const
{
	string s;
	s+=mrpt::format("Status for %u waypoints:\n", static_cast<unsigned int>(waypoints.size()) );
	unsigned int i=0;
	for (auto &wp : waypoints) {
		s+= mrpt::format(" #%3u: ",i++);
		s+= wp.getAsText();
		s+= "\n";
	}
	s+=mrpt::format("final_goal_reached:%s  waypoint_index_current_goal=%d\n", (final_goal_reached ? "YES":"NO "), waypoint_index_current_goal );
	return s;
}

