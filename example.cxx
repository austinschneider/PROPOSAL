#include <iostream>
#include "PROPOSAL/PROPOSAL.h"

using namespace PROPOSAL;

int main(int argc, const char* argv[]) {

    PROPOSAL::InterpolationDef::path_to_tables = "~/.local/share/PROPOSAL/tables/";

    auto geometry = std::make_shared<Sphere>(Vector3D(0., 0., 0.), 1e20, 0);
    auto geometry2 = std::make_shared<Sphere>(Vector3D(0., 0., 1e4), 1e2, 0);
    geometry->SetHierarchy(0);
    geometry2->SetHierarchy(1);

    auto cross = GetStdCrossSections(MuMinusDef(), StandardRock(), std::make_shared<EnergyCutSettings>(500, 0.05, true));
    auto cross2 = GetStdCrossSections(MuMinusDef(), Water(), std::make_shared<EnergyCutSettings>(500, 0.05, true));

    PropagationUtility::Collection collection;
    collection.interaction_calc = make_interaction(cross, true);
    collection.displacement_calc = make_displacement(cross, true);
    collection.decay_calc = make_decay(cross, MuMinusDef(), true);
    collection.time_calc = make_time(cross, MuMinusDef());
    collection.scattering = make_scattering("highland", MuMinusDef(), StandardRock());
    PropagationUtility utility(collection); //unnecessary step, just pass collection around

    PropagationUtility::Collection collection2;
    collection2.interaction_calc = make_interaction(cross2, true);
    collection2.displacement_calc = make_displacement(cross2, true);
    collection2.decay_calc = make_decay(cross2, MuMinusDef(), true);
    collection2.time_calc = make_time(cross2, MuMinusDef());
    collection2.scattering = make_scattering("highland", MuMinusDef(), Water());
    PropagationUtility utility2(collection2); //unnecessary step, just pass collection around

    auto density_distr = std::make_shared<Density_homogeneous>(StandardRock());
    auto density_distr2 = std::make_shared<Density_homogeneous>(Water());

    //auto axis = CartesianAxis(Vector3D(0, 0, 1), Vector3D(0,0,0));
    //auto density_distr_exp = std::make_shared<Density_exponential>(axis, -std::log(0.5) * 200000., Ice().GetMassDensity());

    auto sector1 = std::make_tuple(geometry, utility, density_distr);
    auto sector2 = std::make_tuple(geometry2, utility2, density_distr2);

    auto sectors = std::vector<Sector>{sector1, sector2};

    auto propagator = Propagator(MuMinusDef(), sectors);

    // initial condition
    DynamicData init_state(1000000001, Vector3D(0,0,0), Vector3D(0,0,1), 1e6, 1e6, 0., 0.);

    auto secondaries = propagator.Propagate(init_state);
    for(auto sec: secondaries) {
        std::cout << "d: " << sec.GetPropagatedDistance() << ", E: " << sec.GetEnergy() << std::endl;
    }

/*
    double mean = 0.;
    int statistics = 1000;
    for(int i=0; i<statistics; i++) {
        auto secondaries = propagator.Propagate(init_state);
        std::cout << secondaries.back().GetTime() << std::endl;
        mean += secondaries.back().GetTime();
    }
    std::cout << "mean: " << mean / statistics << std::endl;
*/
    return 0;
}