#pragma once

// API направления

enum DirectionsEnum
{
    left = 0,
    straight = 1,
    right = 2,
    backwards = 3
};

DirectionsEnum opposite_direction(DirectionsEnum direction);
