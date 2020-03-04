
/******************************************************************************
 *                                                                            *
 * This file is part of the simulation tool PROPOSAL.                         *
 *                                                                            *
 * Copyright (C) 2017 TU Dortmund University, Department of Physics,          *
 *                    Chair Experimental Physics 5b                           *
 *                                                                            *
 * This software may be modified and distributed under the terms of a         *
 * modified GNU Lesser General Public Licence version 3 (LGPL),               *
 * copied verbatim in the file "LICENSE".                                     *
 *                                                                            *
 * Modifcations to the LGPL License:                                          *
 *                                                                            *
 *      1. The user shall acknowledge the use of PROPOSAL by citing the       *
 *         following reference:                                               *
 *                                                                            *
 *         J.H. Koehne et al.  Comput.Phys.Commun. 184 (2013) 2070-2090 DOI:  *
 *         10.1016/j.cpc.2013.04.001                                          *
 *                                                                            *
 *      2. The user should report any bugs/errors or improvments to the       *
 *         current maintainer of PROPOSAL or open an issue on the             *
 *         GitHub webpage                                                     *
 *                                                                            *
 *         "https://github.com/tudo-astroparticlephysics/PROPOSAL"            *
 *                                                                            *
 ******************************************************************************/

#pragma once

/*! \mainpage PROPOSAL:
 * <b>PR</b>opagator with <b>O</b>ptimal <b>P</b>recision and <b>O</b>ptimized
 * <b>S</b>peed for <b>A</b>ll <b>L</b>eptons.
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 * \section install_sec Installation
 *
 * \subsection requirements
 *
 * \section HowTo
 *
 */

// #include <deque>
#include <vector>

#include "PROPOSAL/Sector.h"

namespace PROPOSAL {

enum class LossCondition : unsigned short {
    Interaction,
    Decay,
    MinimalE,
    Border
};

enum class BorderCondition : unsigned short {
    Inside,
    Border,
    Outside
};

enum class ContinousCondition : unsigned short {
    Energy,
    Type,
    Distance
};



class ParticlePropagator {
public:
    // Constructors
    ParticlePropagator() = delete;
    ParticlePropagator(const std::vector<SectorDefinition>&);

    ~ParticlePropagator() = default;

    Secondaries Propagate(DynamicData p_condition, double max_distance = 1e20,
        double minimal_energy = 0.);

    bool PropagationStep(DynamicData p_condition);

private:
    void ChooseCurrentSector(
        const Vector3D& particle_position, const Vector3D& particle_direction);

    std::vector<SectorDefinition> sec_def;
};

} // namespace PROPOSAL
