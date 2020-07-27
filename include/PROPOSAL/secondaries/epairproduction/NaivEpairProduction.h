
#pragma once

#include "PROPOSAL/EnergyCutSettings.h"
#include "PROPOSAL/medium/Medium.h"
#include "PROPOSAL/particle/ParticleDef.h"
#include "PROPOSAL/secondaries/epairproduction/EpairProduction.h"

/* #include <unordered_map> */

using PROPOSAL::Components::Component;
/* using std::unordered_map; */
using std::array;
using std::vector;

namespace PROPOSAL {
namespace secondaries {
    struct NaivEpairProduction
        : public secondaries::EpairProduction {
        // unordered_map<Component*, unique_ptr<Interpolant>> dndx;

        static constexpr int n_rnd = 2;
        size_t RequiredRandomNumbers() const noexcept { return n_rnd; }

        NaivEpairProduction(ParticleDef, Medium){};

        double CalculateRho(double, double, const Component&, double) final{};
        tuple<Vector3D, Vector3D> CalculateDirections(
            Vector3D, double, double, double) final{};
        tuple<double, double> CalculateEnergy(double, double) final{};

        vector<Loss::secondary_t> CalculateSecondaries(
            double, Loss::secondary_t, const Component&, vector<double>) {
            auto sec = vector<Loss::secondary_t>{};
            return sec;
        };
    };
} // namespace secondaries
} // namespace PROPOSAL