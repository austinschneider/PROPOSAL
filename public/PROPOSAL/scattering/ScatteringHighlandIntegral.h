
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

#include "PROPOSAL/scattering/Scattering.h"

namespace PROPOSAL {

class Utility;
class UtilityDecorator;
struct InterpolationDef;
/**
 * \brief This class provides the scattering routine provided by moliere.
 *
 * More precise scattering angles will be added soon.
 */
class ScatteringHighlandIntegral : public Scattering
{
public:
    ScatteringHighlandIntegral(const ParticleDef&, const Crosssections&);
    ScatteringHighlandIntegral(const ParticleDef&, const Crosssections&, const InterpolationDef&);

    // Copy constructor
    /* ScatteringHighlandIntegral(const ParticleDef&, const Utility&, const ScatteringHighlandIntegral&); */
    /* ScatteringHighlandIntegral(const ScatteringHighlandIntegral&); */
    ~ScatteringHighlandIntegral() {};

private:
    /* ScatteringHighlandIntegral& operator=(const ScatteringHighlandIntegral&); // Undefined & not allowed */

    /* void print(std::ostream&) const override; */

    array<double, 4> CalculateRandomAngle(double dr, double ei, double ef, const Vector3D& pos, array<double, 4> rnd) const override;
    double CalculateTheta0(double dr, double ei, double ef, const Vector3D& pos);

    std::unique_ptr<UtilityDecorator> scatter_;
};

} // namespace PROPOSAL
