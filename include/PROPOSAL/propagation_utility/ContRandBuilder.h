#include "PROPOSAL/propagation_utility/DisplacementBuilder.h"
#include "PROPOSAL/propagation_utility/ContRand.h"

namespace PROPOSAL {
template <class T, class Cross> class ContRandBuilder : public ContRand {
    double lower_lim;
    T cont_rand_integral;

    template <typename Disp>
    double FunctionToIntegral(Cross&& cross, Disp&& disp, double energy)
    {
        assert(energy >= 0);
        double sum = 0.0;
        for (const auto& crosssections : cross)
            sum += crosssections->CalculatedE2dx(energy);
        return disp.FunctionToIntegral(cross, energy) * sum;
    }

    T BuildContRandIntegral(Cross&& cross)
    {
        auto disp = DisplacementBuilder<UtilityIntegral, Cross>(cross);
        auto cont_rand_func = [this, cross, &disp](double energy) mutable {
            return FunctionToIntegral(cross, disp, energy);
        };
        T cont_rand_integral(cont_rand_func, lower_lim);
        if (typeid(T) == typeid(UtilityInterpolant)) {
            auto hash = CrossSectionVector::GetHash(cross);
            cont_rand_integral.BuildTables("contrand", hash, interpol_def);
        };
        return cont_rand_integral;
    }

public:
    ContRandBuilder(Cross&& cross)
        : lower_lim(CrossSectionVector::GetLowerLim(cross))
        , cont_rand_integral(BuildContRandIntegral(cross))
    {
    }


    double EnergyRandomize(
        double initial_energy, double final_energy, double rnd) override
    {
        assert(initial_energy >= final_energy);
        double variance = cont_rand_integral.Calculate(initial_energy, final_energy);
        return SampleFromGaussian(
            final_energy, std::sqrt(variance), rnd, lower_lim, initial_energy);
    }
};

template <typename T>
std::unique_ptr<ContRand> make_contrand(T&& cross, bool interpolate = true)
{
    if (interpolate)
        return PROPOSAL::make_unique<ContRandBuilder<UtilityInterpolant, T>>(
            std::forward<T>(cross));
    return PROPOSAL::make_unique<ContRandBuilder<UtilityIntegral, T>>(
            std::forward<T>(cross));
}
} // namespace PROPOSAL
