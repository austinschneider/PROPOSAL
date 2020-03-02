
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
#include "PROPOSAL/math/Vector3D.h"
#include <memory>
#include <utility>

using std::array;
using std::tuple;
namespace PROPOSAL {

struct ParticleDef;
class Utility;

enum Directions { AVERAGED, FINAL };

class Scattering {
public:
    Scattering(const ParticleDef&);
    /* Scattering(const Scattering&); */
    virtual ~Scattering();

    /* bool operator==(const Scattering& scattering) const; */
    /* bool operator!=(const Scattering& scattering) const; */

    friend std::ostream& operator<<(std::ostream&, Scattering const&);


    Directions Scatter(double, double, double, const Vector3D&, const Vector3D&,
        const array<double, 4>&);

protected:
    Scattering& operator=(const Scattering&); // Undefined & not allowed

    // Implemented in child classes to be able to use equality operator
    virtual bool compare(const Scattering&) const = 0;
    virtual void print(std::ostream&) const = 0;

    enum RandomDirections : unsigned short { sx, sy, tx, ty };

    virtual array<double, 4> CalculateRandomAngle(
        double, double, double, const Vector3D&, const array<double, 4>&) const
        = 0;

    const ParticleDef& particle_def_;
};

} // namespace PROPOSAL
