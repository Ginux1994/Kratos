//    |  /           | 
//    ' /   __| _` | __|  _ \   __| 
//    . \  |   (   | |   (   |\__ \.
//   _|\_\_|  \__,_|\__|\___/ ____/ 
//                   Multi-Physics  
//
//  License:		 BSD License 
//			 Kratos default license: kratos/license.txt
//
//  Main authors:  Josep Maria Carbonell    
//


#if !defined(KRATOS_MAT_VARIABLES_H_INCLUDED )
#define  KRATOS_MAT_VARIABLES_H_INCLUDED



// System includes
#include <string>
#include <iostream>

// External includes


// Project includes
#include "includes/define.h"
#include "containers/variable.h"
#include "containers/variable_component.h"
#include "containers/vector_component_adaptor.h"
#include "includes/kratos_components.h"
#include "includes/ublas_interface.h"
#include "containers/array_1d.h"
#include "containers/weak_pointer_vector.h"
#include "containers/periodic_variables_container.h"

#undef  KRATOS_EXPORT_MACRO
#define KRATOS_EXPORT_MACRO KRATOS_API

//commented variables are defined in the variables.h and variables.cpp

namespace Kratos
{
  //solution
  KRATOS_DEFINE_VARIABLE( std::string, CONSTITUTIVE_LAW_NAME )
  KRATOS_DEFINE_VARIABLE( bool, IMPLEX ) 

  //elasticity
  //KRATOS_DEFINE_VARIABLE( double, YOUNG_MODULUS )
  //KRATOS_DEFINE_VARIABLE( double, POISSON_RATIO )
  //KRATOS_DEFINE_VARIABLE( double, DENSITY )
  //KRATOS_DEFINE_VARIABLE( double, THICKNESS )
  //KRATOS_DEFINE_VARIABLE( double, EQUIVALENT_YOUNG_MODULUS )
  //KRATOS_DEFINE_VARIABLE( double, BULK_MODULUS )
  KRATOS_DEFINE_VARIABLE( double, SHEAR_MODULUS )
  KRATOS_DEFINE_VARIABLE( double, LAME_MU )
  KRATOS_DEFINE_VARIABLE( double, LAME_LAMBDA )
  KRATOS_DEFINE_VARIABLE( double, C10 )
  KRATOS_DEFINE_VARIABLE( double, C20 )
  KRATOS_DEFINE_VARIABLE( double, C30 )

  //viscosity
  //KRATOS_DEFINE_VARIABLE( double, VISCOSITY )
  
  //damage
  KRATOS_DEFINE_VARIABLE( double, DAMAGE_VARIABLE )
  KRATOS_DEFINE_VARIABLE( double, DAMAGE_THRESHOLD )
  KRATOS_DEFINE_VARIABLE( double, STRENGTH_RATIO )
  KRATOS_DEFINE_VARIABLE( double, FRACTURE_ENERGY )
  KRATOS_DEFINE_VARIABLE( double, RESIDUAL_STRENGTH )

  //plasticity
  KRATOS_DEFINE_VARIABLE( double, PLASTIC_STRAIN )
  KRATOS_DEFINE_VARIABLE( double, DELTA_PLASTIC_STRAIN )
  KRATOS_DEFINE_VARIABLE( double, NORM_ISOCHORIC_STRESS )
  KRATOS_DEFINE_VARIABLE( double, PLASTIC_STRAIN_RATE )
    
  //hardening
  KRATOS_DEFINE_VARIABLE( double, ISOTROPIC_HARDENING_MODULUS )
  KRATOS_DEFINE_VARIABLE( double, KINEMATIC_HARDENING_MODULUS )
  KRATOS_DEFINE_VARIABLE( double, HARDENING_EXPONENT )
  KRATOS_DEFINE_VARIABLE( double, REFERENCE_HARDENING_MODULUS )
  KRATOS_DEFINE_VARIABLE( double, INFINITY_HARDENING_MODULUS )
  KRATOS_DEFINE_VARIABLE( double, SOFTENING_SLOPE )

  //baker-johnson-cook parameters
  KRATOS_DEFINE_VARIABLE( double, JC_PARAMETER_A )
  KRATOS_DEFINE_VARIABLE( double, JC_PARAMETER_B )
  KRATOS_DEFINE_VARIABLE( double, JC_PARAMETER_C )
  KRATOS_DEFINE_VARIABLE( double, JC_PARAMETER_m )
  KRATOS_DEFINE_VARIABLE( double, JC_PARAMETER_n )
  KRATOS_DEFINE_VARIABLE( double, JC_PARAMETER_K ) 
  
  //thermal
  //KRATOS_DEFINE_VARIABLE( double, THERMAL_EXPANSION_COEFFICIENT )
  KRATOS_DEFINE_VARIABLE( double, REFERENCE_CONDUCTIVITY )
  KRATOS_DEFINE_VARIABLE( double, HARDNESS_CONDUCTIVITY )  
  KRATOS_DEFINE_VARIABLE( double, REFERENCE_TEMPERATURE )
  KRATOS_DEFINE_VARIABLE( double, MELD_TEMPERATURE )
  KRATOS_DEFINE_VARIABLE( double, PLASTIC_DISSIPATION )
  KRATOS_DEFINE_VARIABLE( double, DELTA_PLASTIC_DISSIPATION )
  
  //anisotropy
  KRATOS_DEFINE_VARIABLE( double, YOUNG_MODULUS_X )
  KRATOS_DEFINE_VARIABLE( double, YOUNG_MODULUS_Y )
  KRATOS_DEFINE_VARIABLE( double, YOUNG_MODULUS_Z )
  KRATOS_DEFINE_VARIABLE( double, SHEAR_MODULUS_XY )
  KRATOS_DEFINE_VARIABLE( double, SHEAR_MODULUS_YZ )
  KRATOS_DEFINE_VARIABLE( double, SHEAR_MODULUS_XZ )
  KRATOS_DEFINE_VARIABLE( double, POISSON_RATIO_XY )
  KRATOS_DEFINE_VARIABLE( double, POISSON_RATIO_YZ )
  KRATOS_DEFINE_VARIABLE( double, POISSON_RATIO_XZ )
  
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MATERIAL_ORIENTATION_DX )
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MATERIAL_ORIENTATION_DY )
  KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MATERIAL_ORIENTATION_DZ )

  // critical state
  KRATOS_DEFINE_VARIABLE( double, CRITICAL_STATE_LINE )
  KRATOS_DEFINE_VARIABLE( double, PRE_CONSOLIDATION_STRESS )
  KRATOS_DEFINE_VARIABLE( double, OVER_CONSOLIDATION_RATIO )
  KRATOS_DEFINE_VARIABLE( double, INITIAL_SHEAR_MODULUS )
  KRATOS_DEFINE_VARIABLE( double, NORMAL_COMPRESSION_SLOPE )
  KRATOS_DEFINE_VARIABLE( double, SWELLING_SLOPE )
  KRATOS_DEFINE_VARIABLE( double, ALPHA_SHEAR )

  //strain
  //KRATOS_DEFINE_VARIABLE( double, DETERMINANT_F )
  //KRATOS_DEFINE_VARIABLE( Vector, DEFORMATION_GRADIENT);
  KRATOS_DEFINE_VARIABLE( Vector, INITIAL_STRAIN_VECTOR )
    
  KRATOS_DEFINE_VARIABLE( Vector, GREEN_LAGRANGE_STRAIN_VECTOR )
  //KRATOS_DEFINE_VARIABLE( Vector, GREEN_LAGRANGE_STRAIN_TENSOR )
    
  //KRATOS_DEFINE_VARIABLE( Vector, HENCKY_STRAIN_VECTOR);
  //KRATOS_DEFINE_VARIABLE( Matrix, HENCKY_STRAIN_TENSOR);
  
  KRATOS_DEFINE_VARIABLE( Vector, ALMANSI_STRAIN_VECTOR )
  KRATOS_DEFINE_VARIABLE( Matrix, ALMANSI_STRAIN_TENSOR )

  //stress
  KRATOS_DEFINE_VARIABLE( Vector, KIRCHHOFF_STRESS_VECTOR )
  KRATOS_DEFINE_VARIABLE( Matrix, KIRCHHOFF_STRESS_TENSOR )
  //KRATOS_DEFINE_VARIABLE( Vector, CAUCHY_STRESS_VECTOR )
  //KRATOS_DEFINE_VARIABLE( Matrix, CAUCHY_STRESS_TENSOR )    
  //KRATOS_DEFINE_VARIABLE( Vector, PK2_STRESS_VECTOR )    
  //KRATOS_DEFINE_VARIABLE( Matrix, PK2_STRESS_TENSOR )

}  // namespace Kratos.

#undef  KRATOS_EXPORT_MACRO
#define KRATOS_EXPORT_MACRO KRATOS_NO_EXPORT

#endif // KRATOS_MAT_VARIABLES_H_INCLUDED  defined
