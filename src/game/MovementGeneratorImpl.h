/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS and LICENSE files for Copyright information.
 */

#ifndef BLIZZLIKE_MOVEMENTGENERATOR_IMPL_H
#define BLIZZLIKE_MOVEMENTGENERATOR_IMPL_H

#include "MovementGenerator.h"

template<class MOVEMENT_GEN>
inline MovementGenerator*
MovementGeneratorFactory<MOVEMENT_GEN>::Create(void* /*data*/) const
{
    return (new MOVEMENT_GEN());
}
#endif

