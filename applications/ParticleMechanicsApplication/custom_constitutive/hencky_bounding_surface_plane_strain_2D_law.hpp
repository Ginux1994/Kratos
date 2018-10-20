//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		BSD License
//					Kratos default license: kratos/license.txt
//
//  Main authors:    Bodhinanda Chandra
//


#if !defined (KRATOS_HENCKY_BOUNDING_SURFACE_PLASTIC_PLANE_STRAIN_2D_LAW_H_INCLUDED)
#define       KRATOS_HENCKY_BOUNDING_SURFACE_PLASTIC_PLANE_STRAIN_2D_LAW_H_INCLUDED

// System includes

// External includes

// Project includes
#include "custom_constitutive/hencky_plastic_plane_strain_2D_law.hpp"
#include "custom_constitutive/flow_rules/bounding_surface_plastic_flow_rule.hpp"
#include "custom_constitutive/yield_criteria/bounding_surface_yield_criterion.hpp"
#include "custom_constitutive/hardening_laws/bounding_surface_hardening_law.hpp"


namespace Kratos
{
/**
 * Defines a bounding surface plasticity constitutive law for unified non-associative model
 * This model can also be expanded to a more advance model considering viscoplastic effect
 * This material law is defined by the parameters needed by the yield criterion:
 * TO BE DECIDED
 * The functionality is designed for large displacements considering (v) - ln(p_c) profile with particle crushing
 * For reference, please refer to: (Russell&Khalili, 2004)
*/



class HenckyBoundingSurfacePlasticPlaneStrain2DLaw
    : public HenckyElasticPlasticPlaneStrain2DLaw

{
public:
    /**
     * Type Definitions
     */
    typedef ProcessInfo      ProcessInfoType;
    typedef ConstitutiveLaw         BaseType;
    typedef std::size_t             SizeType;

    typedef MPMFlowRule::Pointer                MPMFlowRulePointer;
    typedef MPMYieldCriterion::Pointer    YieldCriterionPointer;
    typedef MPMHardeningLaw::Pointer        HardeningLawPointer;
    typedef Properties::Pointer            PropertiesPointer;

    /**
     * Counted pointer of HyperElasticPlasticJ2PlaneStrain2DLaw
     */

    KRATOS_CLASS_POINTER_DEFINITION( HenckyBoundingSurfacePlasticPlaneStrain2DLaw );

    /**
     * Life Cycle
     */

    /**
     * Default constructor.
     */
    HenckyBoundingSurfacePlasticPlaneStrain2DLaw();


    HenckyBoundingSurfacePlasticPlaneStrain2DLaw(MPMFlowRulePointer pMPMFlowRule, YieldCriterionPointer pYieldCriterion, HardeningLawPointer pHardeningLaw);

    /**
     * Copy constructor.
     */
    HenckyBoundingSurfacePlasticPlaneStrain2DLaw (const HenckyBoundingSurfacePlasticPlaneStrain2DLaw& rOther);


    /**
     * Assignment operator.
     */

    //HyperElasticPlasticJ2PlaneStrain2DLaw& operator=(const HyperElasticPlasticJ2PlaneStrain2DLaw& rOther);

    /**
     * Clone function (has to be implemented by any derived class)
     * @return a pointer to a new instance of this constitutive law
     */
    ConstitutiveLaw::Pointer Clone() const override;

    /**
     * Destructor.
     */
    ~HenckyBoundingSurfacePlasticPlaneStrain2DLaw() override;

    /**
     * Operators
     */

    /**
     * Operations needed by the base class:
     */


    /**
     * This function is designed to be called once to perform all the checks needed
     * on the input provided. Checks can be "expensive" as the function is designed
     * to catch user's errors.
     * @param props
     * @param geom
     * @param CurrentProcessInfo
     * @return
     */
    int Check(const Properties& rProperties, const GeometryType& rGeometry, const ProcessInfo& rCurrentProcessInfo) override;



    /**
     * Input and output
     */
    /**
     * Turn back information as a string.
     */
    //virtual String Info() const;
    /**
     * Print information about this object.
     */
    //virtual void PrintInfo(std::ostream& rOStream) const;
    /**
     * Print object's data.
     */
    //virtual void PrintData(std::ostream& rOStream) const;

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


    ///@}
    ///@name Serialization
    ///@{
    friend class Serializer;

    void save(Serializer& rSerializer) const override
    {
        KRATOS_SERIALIZE_SAVE_BASE_CLASS( rSerializer, HenckyElasticPlasticPlaneStrain2DLaw )
    }

    void load(Serializer& rSerializer) override
    {
        KRATOS_SERIALIZE_LOAD_BASE_CLASS( rSerializer, HenckyElasticPlasticPlaneStrain2DLaw )
    }



}; // Class HenckyBoundingSurfacePlasticPlaneStrain2DLaw
}  // namespace Kratos.
#endif // KRATOS_HENCKY_BOUNDING_SURFACE_PLASTICITY_PLANE_STRAIN_2D_LAW_H_INCLUDED defined
