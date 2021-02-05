
#pragma once

#include "PROPOSAL/crosssection/CrossSectionDEDX/AxisBuilderDEDX.h"
#include "PROPOSAL/crosssection/CrossSectionDEDX/CrossSectionDEDXIntegral.h"

#include <type_traits>

#include "CubicInterpolation/CubicSplines.h"
#include "CubicInterpolation/Interpolant.h"

namespace PROPOSAL {
class EnergyCutSettings;
} // namespace PROPOSAL

namespace PROPOSAL {

double transform_relativ_loss(double v_cut, double v_max, double v);
double retransform_relativ_loss(double v_cut, double v_max, double v);

template <typename T1, typename... Args>
auto build_dedx_def(T1 const& param, ParticleDef const& p, Args... args)
{
    auto dedx = std::make_shared<CrossSectionDEDXIntegral>(param, p, args...);
    auto ax = AxisBuilderDEDX(param.GetLowerEnergyLim(p));
    ax.refine_definition_range([dedx](double E) { return dedx->Calculate(E); });

    auto def = cubic_splines::CubicSplines<double>::Definition();
    def.f_trafo = std::make_unique<cubic_splines::ExpAxis<double>>(1., 0.);
    def.f = [dedx](double E) { return dedx->Calculate(E); };
    def.axis = ax.Create();
    return def;
}

class CrossSectionDEDXInterpolant : public CrossSectionDEDX {
    cubic_splines::Interpolant<cubic_splines::CubicSplines<double>> interpolant;
    double lower_energy_lim;

    std::string gen_name();

public:
    template <typename Param, typename Target>
    CrossSectionDEDXInterpolant(Param const& param, ParticleDef const& p,
        Target const& t, EnergyCutSettings const& cut, size_t hash = 0)
        : CrossSectionDEDX(hash)
        , interpolant(build_dedx_def(param, p, t, cut), "/tmp", gen_name())
        , lower_energy_lim(interpolant.GetDefinition().GetAxis().GetLow())
    {
    }

    double Calculate(double E) const final;
};
} // namespace PROPOSAL
