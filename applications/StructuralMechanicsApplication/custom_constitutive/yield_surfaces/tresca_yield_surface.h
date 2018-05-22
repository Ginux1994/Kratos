// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:         BSD License
//                   license: structural_mechanics_application/license.txt
//
//  Main authors:    Alejandro Cornejo
//

#if !defined(KRATOS_TRESCA_YIELD_SURFACE_H_INCLUDED)
#define  KRATOS_TRESCA_YIELD_SURFACE_H_INCLUDED

// System includes
#include <string>
#include <iostream>

// Project includes
#include "includes/define.h"
#include "includes/serializer.h"
#include "includes/properties.h"
#include "utilities/math_utils.h"

namespace Kratos
{
///@name Kratos Globals
///@{

///@}
///@name Type Definitions
///@{

///@}
///@name  Enum's
///@{

///@}
///@name  Functions
///@{

///@}
///@name Kratos Classes
///@{
/**
 * @class TrescaYieldSurface
 * @ingroup StructuralMechanicsApplication
 * @brief
 * @details
 * @tparam TPlasticPotentialType 
 * @author Alejandro Cornejo
 */
template <class TPlasticPotentialType , class TVoigtSize>
class KRATOS_API(STRUCTURAL_MECHANICS_APPLICATION) TrescaYieldSurface
{
public:
    ///@name Type Definitions
    ///@{

    /// The type of potential plasticity
    typedef typename TPlasticPotentialType PlasticPotentialType;

    /// Counted pointer of TrescaYieldSurface
    KRATOS_CLASS_POINTER_DEFINITION( TrescaYieldSurface );

    ///@}
    ///@name Life Cycle
    ///@{

    /// Initialization constructor.
    TrescaYieldSurface()
    {
    }

    /// Copy constructor
    TrescaYieldSurface(TrescaYieldSurface const& rOther)
    {
    }

    /// Assignment operator
    TrescaYieldSurface& operator=(TrescaYieldSurface const& rOther)
    {
        return *this;
    }

    /// Destructor
    virtual ~TrescaYieldSurface() {};

    ///@}
    ///@name Operators
    ///@{

    ///@}
    ///@name Operations
    ///@{

    static void CalculateEquivalentStress(
        const Vector& StressVector,
        const Vector& StrainVector, 
        double& rEqStress, 
        const Properties& rMaterialProperties
    )
    {
        double I1, J2, J3, LodeAngle;
        Vector Deviator = ZeroVector(TVoigtSize);

        CalculateI1Invariant(StressVector, I1);
        CalculateJ2Invariant(StressVector, I1, Deviator, J2);
        CalculateJ3Invariant(Deviator, J3);
        CalculateLodeAngle(J2, J3, LodeAngle);

        rEqStress = 2.0*std::cos(LodeAngle)*std::sqrt(J2);
    }

    static void GetInitialUniaxialThreshold(const Properties& rMaterialProperties, double& rThreshold)
    {
        rThreshold = std::abs(rMaterialProperties[YIELD_STRESS_T]);  // TODO Check
    }

    static void CalculateDamageParameter(
        const Properties& rMaterialProperties, 
        double& AParameter, 
        const double CharacteristicLength
    )
    {
        const double Gf = rMaterialProperties[FRACTURE_ENERGY];
        const double E  = rMaterialProperties[YOUNG_MODULUS];
        const double sigma_c = rMaterialProperties[YIELD_STRESS_C];

        if (rMaterialProperties[SOFTENING_TYPE] == Exponential)
        {
            AParameter = 1.00 / (Gt*E / (CharacteristicLength * std::pow(sigma_c, 2)) - 0.5);
        }
        else
        {
            
        }
    }

    static void CalculateI1Invariant(const Vector& StressVector, double& rI1)
    {
        rI1 = StressVector[0] + StressVector[1] + StressVector[2];
    }

    static void CalculateI2Invariant(const Vector& StressVector, double& rI2)
    {
        rI2 = (StressVector[0] + StressVector[2])*StressVector[1] + StressVector[0]*StressVector[2] +
            - StressVector[3]*StressVector[3] - StressVector[4]*StressVector[4] - StressVector[5]*StressVector[5];
    }

    static void CalculateI3Invariant(const Vector& StressVector, double& rI3)
    {
        rI3 = (StressVector[1]*StressVector[2] - StressVector[4]*StressVector[4])*StressVector[0] -
            StressVector[1]*StressVector[5]*StressVector[5] - StressVector[2]*StressVector[3]*StressVector[3] +
            2.0*StressVector[3]*StressVector[4]*StressVector[5];
    }

    static void CalculateJ3Invariant(const Vector& Deviator, double& rJ3)
    {
        rJ3 = Deviator[0]*(Deviator[1]*Deviator[2] - Deviator[4]*Deviator[4])  +
			Deviator[3]*(-Deviator[3]*Deviator[2]  + Deviator[5]*Deviator[4])  +
			Deviator[5]*(Deviator[3]*Deviator[4] - Deviator[5]*Deviator[1]);
    }

    static void CalculateJ2Invariant(const Vector& StressVector, const double& I1, Vector& rDeviator, double& rJ2)
    {
        if (TVoigtSize == 6)
        {
            rDeviator = StressVector;
            const double Pmean = I1 / 3.0;

            rDeviator[0] -= Pmean;
            rDeviator[1] -= Pmean;
            rDeviator[2] -= Pmean;

            rJ2 = 0.5*(rDeviator[0]*rDeviator[0] + rDeviator[1]*rDeviator[1] + rDeviator[2]*rDeviator[2]) +
                (rDeviator[3]*rDeviator[3] + rDeviator[4]*rDeviator[4] + rDeviator[5]*rDeviator[5]);
        }
        else
        {
            // 2d
        }

    }

    // Computes dG/dS
    static void CalculatePlasticPotentialDerivative(
        const Vector& StressVector,
        const Vector& Deviator,
        const double J2, 
        Vector& rg,
        const Properties& rMaterialProperties
    )
    {
        TPlasticPotentialType::CalculatePlasticPotentialDerivative(StressVector, Deviator, J2, rg, rMaterialProperties);
    }

    static void CalculateLodeAngle(const double J2, const double J3, double& LodeAngle)
    {
		const double sint3 = (-3.0*std::sqrt(3.0)*J3) / (2.0*J2*std::sqrt(J2));
		if (sint3 < -0.95) sint3 = -1;
		if (sint3 > 0.95)  sint3 =  1; 
		LodeAngle = asin(sint3) / 3.0;
    }

    /*
    This  script  calculates  the derivatives  of the Yield Surf
    according   to   NAYAK-ZIENKIEWICZ   paper International
    journal for numerical methods in engineering vol 113-135 1972.
    As:            DF/DS = c1*V1 + c2*V2 + c3*V3
    */
    static void CalculateYieldSurfaceDerivative(
        const Vector& StressVector, 
        const Vector& Deviator,
        const double J2, 
        Vector& rFFlux,
        const Properties& rMaterialProperties
    )
    {
        Vector FirstVector, SecondVector, ThirdVector;

        CalculateFirstVector(FirstVector);
        CalculateSecondVector(Deviator, J2, SecondVector);
        CalculateThirdVector(Deviator, J2, ThirdVector);

        double J3, LodeAngle;
        CalculateJ3Invariant(Deviator, J3);
        CalculateLodeAngle(J2, J3, LodeAngle);

        const double Checker = std::abs(LodeAngle*57.29577951308);

        double c1, c2, c3;
        c1 = 0.0;

        if (Checker < 29.0)
        {
            c2 = 2.0*(std::cos(LodeAngle) + std::sin(LodeAngle)*std::tan(3.0*LodeAngle));
            c3 = std::sqrt(3.0)*std::sin(LodeAngle) / (J2*std::cos(3.0*LodeAngle));
        }
        else
        {
            c2 = std::sqrt(3.0);
            c3 = 0.0;
        }

        noalias(rFFlux) = c1*FirstVector + c2*SecondVector + c3*ThirdVector;
    }

    static void CalculateFirstVector(Vector& FirstVector)
    {
        FirstVector = ZeroVector(6);
        FirstVector[0] = 1.0;
        FirstVector[1] = 1.0;
        FirstVector[2] = 1.0;

    }

    static void CalculateSecondVector(
        const Vector Deviator, 
        const double J2, 
        Vector& SecondVector
    )
    {
        const double twosqrtJ2 = 2.0*std::sqrt(J2);
        for (int i = 0; i < 6; i++)
        {
            SecondVector[i] = Deviator[i] / (twosqrtJ2);
        }

        SecondVector[3] *= 2.0;
        SecondVector[4] *= 2.0;
        SecondVector[5] *= 2.0;
    }

    static void CalculateThirdVector(
        const Vector Deviator, 
        const double J2, 
        Vector& ThirdVector
    )
    {
        ThirdVector.resize(6);
        const double J2thirds = J2 / 3.0;

        ThirdVector[0] = Deviator[1]*Deviator[2] - Deviator[4]*Deviator[4] + J2thirds;
        ThirdVector[1] = Deviator[0]*Deviator[2] - Deviator[5]*Deviator[5] + J2thirds;
        ThirdVector[2] = Deviator[0]*Deviator[1] - Deviator[3]*Deviator[3] + J2thirds;
        ThirdVector[3] = 2.0*(Deviator[4]*Deviator[5] - Deviator[3]*Deviator[2]);
        ThirdVector[4] = 2.0*(Deviator[3]*Deviator[4] - Deviator[1]*Deviator[5]);
        ThirdVector[5] = 2.0*(Deviator[5]*Deviator[3] - Deviator[0]*Deviator[4]);
    }
    ///@}
    ///@name Access
    ///@{

    ///@}
    ///@name Inquiry
    ///@{

    ///@}
    ///@name Input and output
    ///@{

    ///@}
    ///@name Friends
    ///@{

    ///@}

protected:
    ///@name Protected static Member Variables
    ///@{

    ///@}
    ///@name Protected member Variables
    ///@{

    ///@}
    ///@name Protected Operators
    ///@{

    ///@}
    ///@name Protected Operations
    ///@{

    ///@}
    ///@name Protected  Access
    ///@{

    ///@}
    ///@name Protected Inquiry
    ///@{

    ///@}
    ///@name Protected LifeCycle
    ///@{

    ///@}
private:
    ///@name Static Member Variables
    ///@{

    ///@}
    ///@name Member Variables
    ///@{

    ///@}
    ///@name Private Operators
    ///@{

    ///@}
    ///@name Private Operations
    ///@{

    ///@}
    ///@name Private  Access
    ///@{

    ///@}
    ///@name Private Inquiry
    ///@{

    ///@}
    ///@name Un accessible methods
    ///@{

    // Serialization

    friend class Serializer;

    void save(Serializer& rSerializer) const
    {
    }

    void load(Serializer& rSerializer)
    {
    }

    ///@}

}; // Class TrescaYieldSurface

///@}

///@name Type Definitions
///@{

///@}
///@name Input and output
///@{

///@}

}// namespace Kratos.
#endif
