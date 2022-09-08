#pragma once

// API направления

enum DirectionsEnum
{
    left = 0,
    right = 1,
    straight = 2,
    backwards = 3
};

DirectionsEnum opposite_direction(DirectionsEnum direction)
{
    switch (direction)
    {
    case DirectionsEnum::left:
        return DirectionsEnum::right;
    case DirectionsEnum::right:
        return DirectionsEnum::left;
    case DirectionsEnum::straight:
        return DirectionsEnum::backwards;
    case DirectionsEnum::backwards:
        return DirectionsEnum::straight;
    }
}