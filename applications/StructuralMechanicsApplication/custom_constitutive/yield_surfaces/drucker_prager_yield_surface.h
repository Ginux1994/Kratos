// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:         BSD License
//                   license: structural_mechanics_application/license.txt
//
//  Main authors:    Alejandro Cornejo & Lucia Barbu
//

#if !defined(KRATOS_DRUCKER_PRAGER_YIELD_SURFACE_H_INCLUDED)
#define KRATOS_DRUCKER_PRAGER_YIELD_SURFACE_H_INCLUDED

// System includes

// Project includes
#include "includes/checks.h"
#include "custom_constitutive/yield_surfaces/generic_yield_surface.h"

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
 * @class DruckerPragerYieldSurface
 * @ingroup StructuralMechanicsApplication
 * @brief
 * @details
 * @tparam TPlasticPotentialType The plastic potential considered
 * @tparam TVoigtSize The number of components on the Voigt notation
 * @author Alejandro Cornejo & Lucia Barbu
 */
//template <class TPlasticPotentialType , std::size_t TVoigtSize>
template <class TPlasticPotentialType>
class KRATOS_API(STRUCTURAL_MECHANICS_APPLICATION) DruckerPragerYieldSurface
{
public:
    ///@name Type Definitions
    ///@{

    /// The type of potential plasticity
    typedef TPlasticPotentialType PlasticPotentialType;

    /// Counted pointer of DruckerPragerYieldSurface
    KRATOS_CLASS_POINTER_DEFINITION(DruckerPragerYieldSurface);

    static constexpr double tolerance = std::numeric_limits<double>::epsilon();

    ///@}
    ///@name Life Cycle
    ///@{

    /// Initialization constructor.
    DruckerPragerYieldSurface()
    {
    }

    /// Copy constructor
    DruckerPragerYieldSurface(DruckerPragerYieldSurface const &rOther)
    {
    }

    /// Assignment operator
    DruckerPragerYieldSurface &operator=(DruckerPragerYieldSurface const &rOther)
    {
        return *this;
    }

    /// Destructor
    virtual ~DruckerPragerYieldSurface(){};

    ///@}
    ///@name Operators
    ///@{
    ///@}
    ///@name Operations
    ///@{

    /**
     * @brief This method the uniaxial equivalent stress
     * @param rStressVector The stress vector
     * @param rStrainVector The StrainVector vector
     * @param rMaterialProperties The material properties
     */
    static void CalculateEquivalentStress(
        const Vector& rStressVector,
        const Vector& rStrainVector,
        double& rEqStress,
        const Properties& rMaterialProperties
        )
    {
        double friction_angle = rMaterialProperties[FRICTION_ANGLE] * Globals::Pi / 180.0; // In radians!
        const double sin_phi = std::sin(friction_angle);
        const double root_3 = std::sqrt(3.0);

        // Check input variables
        if (friction_angle < tolerance) {
            friction_angle = 32.0 * Globals::Pi / 180.0;
            KRATOS_WARNING("DruckerPragerYieldSurface") << "Friction Angle not defined, assumed equal to 32 " << std::endl;
        }

        double I1, J2;
        ConstitutiveLawUtilities::CalculateI1Invariant(rStressVector, I1);
        Vector Deviator = ZeroVector(6);
        ConstitutiveLawUtilities::CalculateJ2Invariant(rStressVector, I1, Deviator, J2);

        if (I1 == 0.0) {
            rEqStress = 0.0;
        } else {
            const double CFL = -root_3 * (3.0 - sin_phi) / (3.0 * sin_phi - 3.0);
            const double TEN0 = 2.0 * I1 * sin_phi / (root_3 * (3.0 - sin_phi)) + std::sqrt(J2);
            rEqStress = std::abs(CFL * TEN0);
        }
    }

    /**
     * @brief This method returns the initial uniaxial stress threshold
     * @param rThreshold The uniaxial stress threshold
     * @param rMaterialProperties The material properties
     */
    static void GetInitialUniaxialThreshold(
        const Properties& rMaterialProperties,
        double& rThreshold
        )
    {
        const double yield_tension = rMaterialProperties[YIELD_STRESS_TENSION];
        const double friction_angle = rMaterialProperties[FRICTION_ANGLE] * Globals::Pi / 180.0; // In radians!
        const double sin_phi = std::sin(friction_angle);
        rThreshold = std::abs(yield_tension * (3.0 + sin_phi) / (3.0 * sin_phi - 3.0));
    }

    /**
     * @brief This method returns the damage parameter needed in the exp/linear expressions of damage
     * @param rAParameter The damage parameter
     * @param rMaterialProperties The material properties
     * @param CharacteristicLength The equivalent length of the FE
     */
    static void CalculateDamageParameter(
        const Properties& rMaterialProperties,
        double& rAParameter,
        const double CharacteristicLength)
    {
        const double Gf = rMaterialProperties[FRACTURE_ENERGY];
        const double E = rMaterialProperties[YOUNG_MODULUS];
        const double sigma_c = rMaterialProperties[YIELD_STRESS_COMPRESSION];
        const double sigma_t = rMaterialProperties[YIELD_STRESS_TENSION];
        const double n = sigma_c / sigma_t;

        if (rMaterialProperties[SOFTENING_TYPE] == static_cast<int>(SofteningType::Exponential)) {
            rAParameter = 1.00 / (Gf * n * n * E / (CharacteristicLength * std::pow(sigma_c, 2)) - 0.5);
            KRATOS_ERROR_IF(rAParameter < 0.0) << "Fracture energy is too low, increase FRACTURE_ENERGY..." << std::endl;
        } else { // linear
            rAParameter = -std::pow(sigma_c, 2) / (2.0 * E * Gf * n * n / CharacteristicLength);
        }
    }

    /**
     * @brief This method calculates the derivative of the plastic potential DG/DS
     * @param rStressVector The stress vector
     * @param rDeviator The deviatoric part of the stress vector
     * @param J2 The second invariant of the Deviator
     * @param rGFlux The derivative of the plastic potential
     * @param rMaterialProperties The material properties
     */
    static void CalculatePlasticPotentialDerivative(
        const Vector& rStressVector,
        const Vector& rDeviator,
        const double J2,
        Vector& rGFlux,
        const Properties &rMaterialProperties)
    {
        TPlasticPotentialType::CalculatePlasticPotentialDerivative(rStressVector, rDeviator, J2, rGFlux, rMaterialProperties);
    }

    /**
     * @brief This  script  calculates  the derivatives  of the Yield Surf
    according   to   NAYAK-ZIENKIEWICZ   paper International
    journal for numerical methods in engineering vol 113-135 1972.
     As:            DF/DS = c1*V1 + c2*V2 + c3*V3
     * @param rStressVector The stress vector
     * @param rDeviator The deviatoric part of the stress vector
     * @param J2 The second invariant of the Deviator
     * @param rFFlux The derivative of the yield surface
     * @param rMaterialProperties The material properties
     */
    static void CalculateYieldSurfaceDerivative(
        const Vector& rStressVector,
        const Vector& rDeviator,
        const double J2,
        Vector& rFFlux,
        const Properties& rMaterialProperties
        )
    {
        Vector first_vector, second_vector, third_vector;
        ConstitutiveLawUtilities::CalculateFirstVector(first_vector);
        ConstitutiveLawUtilities::CalculateSecondVector(rDeviator, J2, second_vector);
        ConstitutiveLawUtilities::CalculateThirdVector(rDeviator, J2, third_vector);

        double c1, c2, c3;
        c3 = 0.0;

        const double friction_angle = rMaterialProperties[FRICTION_ANGLE];
        const double sin_phi = std::sin(friction_angle);
        const double Root3 = std::sqrt(3.0);

        const double CFL = -Root3 * (3.0 - sin_phi) / (3.0 * sin_phi - 3.0);
        c1 = CFL * 2.0 * sin_phi / (Root3 * (3.0 - sin_phi));
        c2 = CFL;

        noalias(rFFlux) = c1 * first_vector + c2 * second_vector + c3 * third_vector;
    }

    /**
     * @brief This method defines the check to be performed in the yield surface
     * @return 0 if OK, 1 otherwise
     */
    static int Check(const Properties& rMaterialProperties)
    {
        KRATOS_CHECK_VARIABLE_KEY(FRICTION_ANGLE);
        KRATOS_CHECK_VARIABLE_KEY(YIELD_STRESS_TENSION);
        KRATOS_CHECK_VARIABLE_KEY(YIELD_STRESS_COMPRESSION);
        KRATOS_CHECK_VARIABLE_KEY(FRACTURE_ENERGY);
        KRATOS_CHECK_VARIABLE_KEY(YOUNG_MODULUS);

        KRATOS_ERROR_IF_NOT(rMaterialProperties.Has(FRICTION_ANGLE)) << "FRICTION_ANGLE is not a defined value" << std::endl;
        KRATOS_ERROR_IF_NOT(rMaterialProperties.Has(YIELD_STRESS_TENSION)) << "YIELD_STRESS_TENSION is not a defined value" << std::endl;
        KRATOS_ERROR_IF_NOT(rMaterialProperties.Has(YIELD_STRESS_COMPRESSION)) << "YIELD_STRESS_COMPRESSION is not a defined value" << std::endl;
        KRATOS_ERROR_IF_NOT(rMaterialProperties.Has(FRACTURE_ENERGY)) << "FRACTURE_ENERGY is not a defined value" << std::endl;
        KRATOS_ERROR_IF_NOT(rMaterialProperties.Has(YOUNG_MODULUS)) << "YOUNG_MODULUS is not a defined value" << std::endl;

        return TPlasticPotentialType::Check(rMaterialProperties);
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

    void save(Serializer &rSerializer) const
    {
    }

    void load(Serializer &rSerializer)
    {
    }

    ///@}

}; // Class DruckerPragerYieldSurface

///@}

///@name Type Definitions
///@{

///@}
///@name Input and output
///@{

///@}

} // namespace Kratos.
#endif
