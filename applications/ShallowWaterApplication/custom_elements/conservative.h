//   
//   Project Name:        Kratos       
//   Last modified by:    $Author:  Miguel Masó Sotomayor $
//   Date:                $Date:              july 3 2017 $
//   Revision:            $Revision:                  1.1 $
//
//

#if !defined(KRATOS_CONSERVATIVE_ELEM_H_INCLUDED)
#define  KRATOS_CONSERVATIVE_ELEM_H_INCLUDED 

// System includes 


// External includes 
#include "boost/smart_ptr.hpp"


// Project includes
#include "includes/define.h"
#include "includes/element.h"
#include "includes/ublas_interface.h"
#include "includes/variables.h" 

namespace Kratos
{

  class Conservative : public Element
  {
    public:
     
    /// Counted pointer of Conservative
    KRATOS_CLASS_POINTER_DEFINITION(Conservative);


    /// Default constructor.
    Conservative(IndexType NewId, GeometryType::Pointer pGeometry);
    Conservative(IndexType NewId, GeometryType::Pointer pGeometry, PropertiesType::Pointer pProperties);

    /// Destructor.
    virtual ~ Conservative();


    Element::Pointer Create(IndexType NewId, NodesArrayType const& ThisNodes,  PropertiesType::Pointer pProperties) const;

    void CalculateLocalSystem(MatrixType& rLeftHandSideMatrix, VectorType& rRightHandSideVector, ProcessInfo& rCurrentProcessInfo);

    void CalculateRightHandSide(VectorType& rRightHandSideVector, ProcessInfo& rCurrentProcessInfo);

    void EquationIdVector(EquationIdVectorType& rResult, ProcessInfo& rCurrentProcessInfo);

    void GetDofList(DofsVectorType& rElementalDofList,ProcessInfo& rCurrentProcessInfo);

    void InitializeSolutionStep(ProcessInfo& rCurrentProcessInfo);

    void GetValueOnIntegrationPoints(const Variable<double>& rVariable, std::vector<double>& rValues, const ProcessInfo& rCurrentProcessInfo);



    protected:

    void CalculateConsistentMassMatrix(boost::numeric::ublas::bounded_matrix<double,9,9>& rMassMatrix);

    void CalculateLumpedMassMatrix(boost::numeric::ublas::bounded_matrix<double,9,9>& rMassMatrix);

    private:

    friend class Serializer;

    // A private default constructor necessary for serialization
    Conservative() : Element()
    {
    }
       
       
  }; // Class Conservative
}  // namespace Kratos.

#endif // KRATOS_CONSERVATIVE_ELEM_H_INCLUDED  defined
