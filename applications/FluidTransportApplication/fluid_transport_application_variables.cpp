#include "fluid_transport_application_variables.h"

namespace Kratos
{
KRATOS_CREATE_VARIABLE(double,PECLET)
KRATOS_CREATE_VARIABLE(double,THETA)
KRATOS_CREATE_VARIABLE(double,DT_PHI)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PHI_GRADIENT)

}
