/*
 * Propagator.cxx
 *
 *  Created on: 23.04.2013
 *      Author: koehne
 */

// #include <cmath>

#include <array>
#include <memory>
#include <tuple>
#include <algorithm>

#include "PROPOSAL/Propagator.h"
#include "PROPOSAL/json.hpp"

using namespace PROPOSAL;
using std::array;
using std::numeric_limits;
using std::tuple;
using std::minmax;
using std::copy;

ParticlePropagator::ParticlePropagator(
    const std::vector < SectorDefinition & sec_def)
    : sec_def(sec_def)
{
}

tuple<double, unsigned short, double> ParticlePropagator::LengthContinuousSuggestion(double initial_energy, const PropagationUtility& prop, const array<double, 2>& rnd)
{
    array<double, 4> loss;
    loss[LossCondition::Decay] = prop.EnergyDecay(initial_energy, rnd[0]);
    loss[LossCondition::Interaction] = prop.EnergyInteraction(initial_energy, rnd[1]);
    loss[LossCondition::MaxDistance] = prop.EnergyDistance(initial_energy, max_dist);
    loss[LossCondition::MiniEnergy] = minimal_energy;

    auto min_loss = *minmax(loss.begin(), loss.end()).first;
    const auto& loss_energy = loss[min_loss];

    auto loss_distance = prop.LengthContinuous(initial_energy, loss_energy, numeric_limits<double>::infinity);

    return std::make_tuple(loss_energy, min_loss, loss_distance);
}


DynamicData ParticlePropagator::DoContinuous(DynamicData condition, const array<double, 6>& rnd)
{
    array<double, 2> rnd_sub;
    copy(rnd.begin(), rnd.begin()+2, rnd_sub.begin());
    auto continuous_sugestion = LengthContinuousSuggestion(condition.GetEnergy(), prop, rnd);

    const auto& initial_energy = condition.GetEnergy();
    const auto& pos = condition.GetPosition();
    const auto& dir = condition.GetDirection();
    const auto& type = std::get<ContinuousCondition::Type>(continuous_sugestion);

    auto final_energy = std::get<ContinuousCondition::Energy>(continuous_sugestion);
    auto disp = std::get<ContinuousCondition::Distance>(continuous_sugestion);
    auto directions = prop.DirectionsScatter(disp, initial_energy, final_energy, pos,  dir, rnd);

    if (!IsInsideSector(pos + disp * std::get<Directions::Mean>(directions)))
        SetBorderCondition(initial_energy, final_energy, pos, dir, rnd);

    pos += disp * dir;
    time += prop.ElapsedTime(final_energy, rnd);
    final_energy = prop.EnergyRandomize(final_energy, rnd);

    condition.SetType(InteractionType::Continuous);
    condition.SetParentParticleEnergy(initial_energy);
    condition.SetEnergy(final_energy);
    condition.SetTime(time);
    condition.SetPosition(pos);
    condition.SetDirection(dir);

    return condition;
}

DynamicData ParticlePropagator::DoStochastic(DynamicData condition, const PropagationUtility& prop, const array<double, 3>& rnd)
{
    auto initial_energy = condition.GetEnergy();
    auto direction = condition.GetDirection();

    const auto& cross = prop.TypeInteraction(initial_energy, rnd[0]);
    const auto& interaction_type = cross.GetType();

    array<double, 2> rnd_sub;
    copy(rnd.begin()+1, rnd.end(), rnd_sub.begin());

    auto loss_energy = prop.EnergyStochasticloss(cross, initial_energy, rnd_sub);
    auto deflection = prop.DirectionDeflect(cross, initial_energy, loss_energy);


    condition.SetType(interaction_type);
    condition.SetEnergy(initial_energy - loss_energy);
    condition.SetDirection(direction.deflect(deflection));

    return condition;
}

