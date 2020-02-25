
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

#include <string>
#include <vector>
#include <memory>

#include "PROPOSAL/decay/DecayTable.h"
#include "PROPOSAL/methods.h"

#define PARTICLE_DEF(cls)                                  \
    struct cls##Def : public ParticleDef                   \
    {                                                      \
        cls##Def();                                        \
        ~cls##Def();                                       \
    };

namespace PROPOSAL {
    typedef std::vector<std::vector<const double> > HardComponentTables;

    enum class CrosssectionType {
        BremsStrahlung
    };
} // namespace PROPOSAL

namespace PROPOSAL {
enum class ParticleType : int
{
    None = 0,
    EMinus = 11,
    EPlus = -11,
    NuE = 12,
    NuEBar = -12,
    MuMinus = 13,
    MuPlus = -13,
    NuMu = 14,
    NuMuBar = -14,
    TauMinus = 15,
    TauPlus = -15,
    NuTau = 16,
    NuTauBar = -16,
    Gamma = 22,
    Pi0 = 111,
    PiPlus  = 211,
    PiMinus = -211,
    K0 = 311,
    KPlus = 321,
    KMinus= -321,
    STauMinus = 1000015,
    STauPlus= -1000015,
    PPlus = 2212,
    PMinus = -2212,
    Monopole = 41,
    SMPPlus = 9500,
    SMPMinus = -9500,
};
} // namespace PROPOSAL


// ----------------------------------------------------------------------------
/// @brief Struct to define Basic Particle Properties
///
/// Used to construct Particles
// ----------------------------------------------------------------------------
namespace PROPOSAL {
struct ParticleDef
{
    ParticleDef(std::string name,
                double mass,
                double low,
                double lifetime,
                double charge,
                const ParticleType,
                const ParticleType,

                std::unique_ptr<std::vector<CrosssectionType>> ,
                std::unique_ptr<HardComponentTables>,
                std::unique_ptr<DecayTable>
                );

    bool operator==(const ParticleDef&) const;
    bool operator!=(const ParticleDef&) const;

    friend std::ostream& operator<<(std::ostream&, ParticleDef const&);

    std::string name;
    double mass;
    double low; //!< energy below which the particle is lost [MeV]
    double lifetime;
    double charge;
    ParticleType particle_type;
    ParticleType weak_partner;

    std::unique_ptr<std::vector<CrosssectionType>> crosssections;
    std::unique_ptr<HardComponentTables> hard_table;
    std::unique_ptr<DecayTable> decay_table;
};


std::ostream& operator<<(std::ostream&, PROPOSAL::ParticleDef const&);


// ------------------------------------------------------------------------- //
// Predefined particle definitions
// ------------------------------------------------------------------------- //

PARTICLE_DEF(MuMinus)
PARTICLE_DEF(MuPlus)

PARTICLE_DEF(EMinus)
PARTICLE_DEF(EPlus)

PARTICLE_DEF(TauMinus)
PARTICLE_DEF(TauPlus)

PARTICLE_DEF(StauMinus)
PARTICLE_DEF(StauPlus)

PARTICLE_DEF(Pi0)
PARTICLE_DEF(PiMinus)
PARTICLE_DEF(PiPlus)

// ----------------------------------------------------------------------------
/// @brief Particle definition of K0
///
/// K0 is defined with a liftime of -1, meaning stable.
/// The reason is, that PROPOSAL uses this particle soly as
/// return value from decays and does therefore not decide between
/// K short and K long.
///
/// Used to construct Particles
// ----------------------------------------------------------------------------
PARTICLE_DEF(K0)

PARTICLE_DEF(KMinus)
PARTICLE_DEF(KPlus)

PARTICLE_DEF(PMinus)
PARTICLE_DEF(PPlus)

PARTICLE_DEF(NuE)
PARTICLE_DEF(NuEBar)

PARTICLE_DEF(NuMu)
PARTICLE_DEF(NuMuBar)

PARTICLE_DEF(NuTau)
PARTICLE_DEF(NuTauBar)

PARTICLE_DEF(Monopole)
PARTICLE_DEF(Gamma)

PARTICLE_DEF(SMPMinus)
PARTICLE_DEF(SMPPlus)

} // namespace PROPOSAL


PROPOSAL_MAKE_HASHABLE(PROPOSAL::ParticleDef, t.mass, t.lifetime, t.charge)

#undef PARTICLE_DEF

namespace PROPOSAL {
static std::map<ParticleType, const ParticleDef&> Type_Particle_Map
{
    {ParticleType::EMinus, EMinusDef()},
    {ParticleType::EPlus, EPlusDef()},
    {ParticleType::NuE, NuEDef()},
    {ParticleType::NuEBar, NuEBarDef()},
    {ParticleType::MuMinus, MuMinusDef()},
    {ParticleType::NuMu, NuMuDef()},
    {ParticleType::NuMuBar, NuMuBarDef()},
    {ParticleType::MuPlus, MuPlusDef()},
    {ParticleType::TauMinus, TauMinusDef()},
    {ParticleType::TauPlus, TauPlusDef()},
    {ParticleType::NuTau, NuTauDef()},
    {ParticleType::NuTauBar, NuTauBarDef()},
    {ParticleType::Gamma, GammaDef()},
    {ParticleType::Pi0, Pi0Def()},
    {ParticleType::PiPlus, PiPlusDef()},
    {ParticleType::PiMinus ,PiMinusDef()},
    {ParticleType::K0, K0Def()},
    {ParticleType::KPlus, KPlusDef()},
    {ParticleType::KMinus, KMinusDef()},
    {ParticleType::PPlus, PPlusDef()},
    {ParticleType::PMinus, PMinusDef()},
    {ParticleType::Monopole, MonopoleDef()},
    {ParticleType::SMPPlus, SMPPlusDef()},
    {ParticleType::SMPMinus, SMPMinusDef()},
};
} // namespace PROPOSAL
