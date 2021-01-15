#pragma once

#include "PROPOSAL/math/Integral.h"
#include "PROPOSAL/medium/Components.h"
#include "PROPOSAL/secondaries/parametrization/photopairproduction/PhotopairProduction.h"

#include "PROPOSAL/crosssection/CrossSection.h"
#include "PROPOSAL/crosssection/CrossSectionDNDX/CrossSectionDNDXBuilder.h"
#include "PROPOSAL/crosssection/parametrization/PhotoPairProduction.h"

using PROPOSAL::Components::Component;

namespace PROPOSAL {
namespace secondaries {
    class PhotoTsai : public PhotopairProduction,
                      public DefaultSecondaries<PhotoTsai> {

        using comp_ptr = std::shared_ptr<const Component>;
        using dndx_ptr = std::unique_ptr<CrossSectionDNDX>;
        std::unordered_map<comp_ptr, std::tuple<double, dndx_ptr>> dndx;

        Integral integral;
        Medium medium;

        double FunctionToIntegral(
            double energy, double x, double theta, const Component&);

    public:
        static constexpr int n_rnd = 5;

        PhotoTsai() = default;
        PhotoTsai(ParticleDef p, Medium m)
            : medium(m)
            , dndx(detail::build_dndx(std::true_type {}, false, medium,
                  crosssection::PhotoPairTsai(), p))
        {
        }

        double CalculateRho(double, double, const Component&) override;
        std::tuple<Vector3D, Vector3D> CalculateDirections(Vector3D, double,
            double, const Component&, std::vector<double>) override;
        std::tuple<double, double> CalculateEnergy(
            double, double, double) override;

        size_t RequiredRandomNumbers() const noexcept final { return n_rnd; }
        std::vector<ParticleState> CalculateSecondaries(
            StochasticLoss, const Component&, std::vector<double>&) final;
    };
} // namespace secondaries
} // namespace PROPOSAL
