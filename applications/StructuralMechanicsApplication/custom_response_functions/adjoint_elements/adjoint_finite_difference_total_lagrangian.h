// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:     BSD License
//           license: structural_mechanics_application/license.txt
//
//  Main authors:    Martin Fusseder, https://github.com/MFusseder
//


#if !defined(ADJOINT_FINITE_DIFFERENCE_TOTAL_LAGRANGIAN_H_INCLUDED )
#define  ADJOINT_FINITE_DIFFERENCE_TOTAL_LAGRANGIAN_H_INCLUDED

#include "adjoint_finite_difference_base_element.h"

namespace Kratos
{

class AdjointFiniteDifferenceTotalLagrangian : public AdjointFiniteDifferencingBaseElement
{
public:
    KRATOS_CLASS_POINTER_DEFINITION(AdjointFiniteDifferenceTotalLagrangian);

    AdjointFiniteDifferenceTotalLagrangian(): AdjointFiniteDifferencingBaseElement()
    {
    }

    AdjointFiniteDifferenceTotalLagrangian(Element::Pointer pPrimalElement);

    ~AdjointFiniteDifferenceTotalLagrangian() override;

    void EquationIdVector(EquationIdVectorType& rResult, ProcessInfo& rCurrentProcessInfo) override;

    void GetDofList(DofsVectorType& ElementalDofList, ProcessInfo& CurrentProcessInfo) override;

    void GetValuesVector(Vector& values, int Step = 0) override;

    void Calculate(const Variable<Vector >& rVariable,
                        Vector& rOutput,
                        const ProcessInfo& rCurrentProcessInfo) override;

    int Check(const ProcessInfo& rCurrentProcessInfo) override;

protected:


private:
    double GetPerturbationSizeModificationFactor(const Variable<array_1d<double,3>>& rDesignVariable) override;

    friend class Serializer;
    void save(Serializer& rSerializer) const override;
    void load(Serializer& rSerializer) override;
};


}

#endif
