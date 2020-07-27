
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

#include "PROPOSAL/crosssection/parametrization/Parametrization.h"

#include <memory>
#include <unordered_map>
#include <vector>

using std::unordered_map;

namespace PROPOSAL {
class Interpolant;

namespace crosssection {
    class WeakInteraction : public Parametrization {
    public:
        WeakInteraction();
        ~WeakInteraction() = default;

        using only_stochastic = std::true_type;
        using component_wise = std::true_type;

        double GetLowerEnergyLim(const ParticleDef&) const noexcept override;
        tuple<double, double> GetKinematicLimits(const ParticleDef&,
            const Component&, double) const noexcept override;
    };

    class WeakCooperSarkarMertsch : public WeakInteraction {
        unordered_map<bool,
            tuple<std::unique_ptr<Interpolant>, unique_ptr<const Interpolant>>>
            interpolant_;
        tuple<Interpolant, Interpolant> BuildContribution(
            bool is_decayable) const;

    public:
        WeakCooperSarkarMertsch();

        using base_param_t = WeakInteraction;

        double DifferentialCrossSection(const ParticleDef&, const Component&,
            double, double) const override;
    };

} // namespace crosssection
} // namespace PROPOSAL