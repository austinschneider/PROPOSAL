#pragma once

#include "PROPOSAL/EnergyCutSettings.h"
#include "PROPOSAL/crosssection/CrossSectionDE2DX/CrossSectionDE2DX.h"
#include "PROPOSAL/crosssection/parametrization/Parametrization.h"
#include "PROPOSAL/math/Integral.h"
#include "PROPOSAL/medium/Medium.h"

namespace PROPOSAL {

namespace detail {
    template <typename T1>
    auto define_de2dx_integral(T1 param, ParticleDef const& p_def,
        Component const& comp, EnergyCutSettings const& cut)
    {
        return [param, p_def, comp, cut](Integral& i, double E) {
            auto lim = param.GetKinematicLimits(p_def, comp, E);
            auto v_cut = cut.GetCut(lim, E);
            auto dE2dx = [&param, &p_def, &comp, E](double v) {
                return param.FunctionToDE2dxIntegral(p_def, comp, E, v);
            };
            return i.Integrate(
                std::get<crosssection::Parametrization::V_MIN>(lim), v_cut,
                dE2dx, 2);
        };
    }
}

class CrossSectionDE2DXIntegral : public CrossSectionDE2DX {
    Integral integral;
    std::function<double(Integral&, double)> de2dx_integral;

    template <typename T1>
    auto define_integral(T1 param, ParticleDef const& p_def,
        Medium const& medium, EnergyCutSettings const& cut)
    {
        return [param, p_def, medium, cut](Integral& i, double E) {
            auto lim = param.GetKinematicLimits(p_def, medium, E);
            auto v_cut = cut.GetCut(lim, E);
            auto dE2dx = [&param, &p_def, &medium, E](double v) {
                return param.FunctionToDE2dxIntegral(p_def, medium, E, v);
            };
            return i.Integrate(
                std::get<crosssection::Parametrization::V_MIN>(lim), v_cut,
                dE2dx, 2);
        };
    }

public:
    template <typename... Args>
    CrossSectionDE2DXIntegral(Args... args)
        : CrossSectionDE2DX(args...)
        , de2dx_integral(detail::define_de2dx_integral(args...))
    {
    }

    double Calculate(double E) override { return de2dx_integral(integral, E); }
};
} // namespace PROPOSAL
