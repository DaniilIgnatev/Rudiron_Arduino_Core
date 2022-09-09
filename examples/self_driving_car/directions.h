#pragma once

// API направления

enum DirectionsEnum
{
    left = 0,
    right = 1,
    straight = 2,
    backwards = 3
};

DirectionsEnum opposite_direction(DirectionsEnum direction);
