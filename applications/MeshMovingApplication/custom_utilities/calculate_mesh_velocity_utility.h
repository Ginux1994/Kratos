//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Philipp Bucher
//                   Andreas Winterstein
//


#if !defined(KRATOS_CALCULATE_MESH_VELOCITY_UTILITY_H_INCLUDED )
#define  KRATOS_CALCULATE_MESH_VELOCITY_UTILITY_H_INCLUDED


// System includes


// External includes


// Project includes
#include "includes/define.h"
#include "includes/model_part.h"
#include "includes/kratos_parameters.h"


namespace Kratos
{
///@addtogroup MeshMovingApplication
///@{

///@}
///@name Kratos Classes
///@{

/// Short class definition.
/** Detail class definition.
 */
class CalculateMeshVelocityUtility
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of CalculateMeshVelocityUtility
    KRATOS_CLASS_POINTER_DEFINITION(CalculateMeshVelocityUtility);

    typedef std::size_t SizeType;

    ///@name  Enum's
    ///@{

    enum IntegrationMethod{
        bdf1,
        bdf2,
        generalized_alpha,
        newmark,
        bossak
    };

    ///@}

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor.
    CalculateMeshVelocityUtility(ModelPart& rModelPart,
                                 Parameters Settings);

    /// Destructor.
    virtual ~CalculateMeshVelocityUtility() = default;

    ///@}
    ///@name Operators
    ///@{

    /// Assignment operator.
    CalculateMeshVelocityUtility& operator=(CalculateMeshVelocityUtility const& rOther) = delete;

    /// Copy constructor.
    CalculateMeshVelocityUtility(CalculateMeshVelocityUtility const& rOther) = delete;

    ///@}
    ///@name Operations
    ///@{

    void CalculateMeshVelocities();

    static SizeType GetMinimumBufferSize(const std::string& rIntegrationMethod);

    ///@}
    ///@name Access
    ///@{

    ///@}

protected:
    ///@name Protected member Variables
    ///@{

    ///@}
    ///@name Protected Operations
    ///@{

    ///@}
    ///@name Protected  Access
    ///@{

    ///@}

private:
    ///@name Member Variables
    ///@{

    ModelPart& mrModelPart;

    IntegrationMethod mIntegrationMethod;

    double mBossakBeta;
    double mBossakGamma;

    ///@}
    ///@name Private Operations
    ///@{

    void CalculateMeshVelocitiesBDF(const double DeltaTime);

    void CalculateMeshVelocitiesGeneralizedAlpha(const double DeltaTime);

    ///@}
    ///@name Private  Access
    ///@{

    ///@}

}; // Class CalculateMeshVelocityUtility

///@}

///@} addtogroup block

}  // namespace Kratos.

#endif // KRATOS_CALCULATE_MESH_VELOCITY_UTILITY_H_INCLUDED  defined
