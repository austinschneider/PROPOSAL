#pragma once

#include "PROPOSAL/EnergyCutSettings.h"
#include "PROPOSAL/crosssection/CrossSection.h"
#include "PROPOSAL/crosssection/parametrization/Annihilation.h"
#include "PROPOSAL/medium/Medium.h"
#include "PROPOSAL/particle/ParticleDef.h"
#include "PROPOSAL/secondaries/parametrization/annihilation/Annihilation.h"

using PROPOSAL::Components::Component;

namespace PROPOSAL {
namespace secondaries {
    class SingleDifferentialAnnihilation : public secondaries::Annihilation {
        using comp_ptr = std::shared_ptr<const Component>;
        using dndx_ptr = std::unique_ptr<CrossSectionDNDX>;

        Medium m;
        std::unordered_map<comp_ptr, std::tuple<double, dndx_ptr>> dndx;

    public:
        static constexpr int n_rnd = 2;

        template <typename Param>
        SingleDifferentialAnnihilation(Param&& param, const ParticleDef& p,
            const Medium& medium, bool interpol)
            : m(medium)
            , dndx(detail::build_dndx(std::true_type {}, interpol, medium,
                  std::forward<Param>(param), p))
        {
        }

        double CalculateRho(double, double, const Component&) final;
        std::tuple<Vector3D, Vector3D> CalculateDirections(
            Vector3D, double, double, double) final;
        std::tuple<double, double> CalculateEnergy(double, double) final;

        size_t RequiredRandomNumbers() const noexcept { return n_rnd; }
        std::vector<ParticleState> CalculateSecondaries(
            StochasticLoss, const Component&, std::vector<double>&);
    };
} // namespace secondaries
} // namespace PROPOSAL
