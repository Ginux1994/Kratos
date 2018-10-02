//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    
//

#if !defined(KRATOS_SENSITIVITY_BUILDER_H_INCLUDED)
#define KRATOS_SENSITIVITY_BUILDER_H_INCLUDED

// System includes
#include <vector>
#include <string>

// External includes

// Project includes
#include "includes/define.h"
#include "includes/kratos_parameters.h"
#include "utilities/variable_utils.h"
#include "response_functions/adjoint_response_function.h"
#include "solving_strategies/response_functions/nodal_sensitivity_builder.h"
#include "solving_strategies/response_functions/element_sensitivity_builder.h"

namespace Kratos
{
///@name Kratos Classes
///@{

class SensitivityBuilder
{
public:
    ///@name Type Definitions
    ///@{

    KRATOS_CLASS_POINTER_DEFINITION(SensitivityBuilder);

    ///@}
    ///@name Life Cycle
    ///@{

    /// Constructor.
    SensitivityBuilder(Parameters const& rParameters, ModelPart& rModelPart, AdjointResponseFunction::Pointer pResponseFunction)
    : mrModelPart(rModelPart), mpResponseFunction(pResponseFunction)
    {
        KRATOS_TRY;

        Parameters default_settings(R"(
        {
            "sensitivity_model_part_name": "PLEASE_SPECIFY_SENSITIVITY_MODEL_PART",
            "nodal_sensitivity_variables": [],
            "element_sensitivity_variables" : [],
            "integrate_in_time": true
        })");

        Parameters custom_settings = rParameters;
        custom_settings.ValidateAndAssignDefaults(default_settings);

        auto sensitivity_model_part_name = custom_settings["sensitivity_model_part_name"].GetString();
        if (sensitivity_model_part_name != "PLEASE_SPECIFY_SENSITIVITY_MODEL_PART")
            mpSensitivityModelPart =
                &mrModelPart.GetSubModelPart(sensitivity_model_part_name);
        else
            mpSensitivityModelPart = &mrModelPart;

        Parameters nodal_sensitivity_variables = custom_settings["nodal_sensitivity_variables"];
        mNodalSensitivityVariables.resize(nodal_sensitivity_variables.size());
        for (unsigned int i = 0; i < nodal_sensitivity_variables.size(); ++i)
            mNodalSensitivityVariables[i] = nodal_sensitivity_variables[i].GetString();
        
        Parameters element_sensitivity_variables = custom_settings["element_sensitivity_variables"];
        mElementSensitivityVariables.resize(element_sensitivity_variables.size());
        for (unsigned int i = 0; i < element_sensitivity_variables.size(); ++i)
            mElementSensitivityVariables[i] = element_sensitivity_variables[i].GetString();

        mIntegrateInTime = custom_settings["integrate_in_time"].GetBool();

        KRATOS_CATCH("");
    }

    ///@}
    ///@name Operators
    ///@{

    ///@}
    ///@name Operations
    ///@{

    void Initialize()
    {
        KRATOS_TRY;

        for (const std::string& r_label : mNodalSensitivityVariables)
            SetNodalSensitivityVariableToZero(r_label);

        for (const std::string& r_label : mElementSensitivityVariables)
            SetElementSensitivityVariableToZero(r_label);

        VariableUtils().SetNonHistoricalVariable(UPDATE_SENSITIVITIES, false,
                                                 mrModelPart.Nodes());
        VariableUtils().SetNonHistoricalVariable(
            UPDATE_SENSITIVITIES, true,
            mpSensitivityModelPart->Nodes());

        KRATOS_CATCH("");
    }

    void UpdateSensitivities()
    {
        KRATOS_TRY;

        double delta_time;
        if (mIntegrateInTime)
            delta_time = -mrModelPart.GetProcessInfo()[DELTA_TIME];
        else
            delta_time = 1.0;

        NodalSensitivityBuilder nodal_sensitivity_builder(mrModelPart, mpResponseFunction);
        for (const std::string& r_label : mNodalSensitivityVariables)
            nodal_sensitivity_builder.BuildNodalSolutionStepSensitivities(r_label, delta_time);
        
        ElementSensitivityBuilder element_sensitivity_builder(mrModelPart, mpResponseFunction);
        for (const std::string& r_label : mElementSensitivityVariables)
            element_sensitivity_builder.BuildElementSensitivities(r_label, delta_time);

        KRATOS_CATCH("");
    }

    ///@}

protected:
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
    ///@name Member Variables
    ///@{

    ModelPart& mrModelPart;
    ModelPart* mpSensitivityModelPart = nullptr;
    AdjointResponseFunction::Pointer mpResponseFunction;
    std::vector<std::string> mNodalSensitivityVariables;
    std::vector<std::string> mElementSensitivityVariables;
    bool mIntegrateInTime;

    ///@}
    ///@name Private Operators
    ///@{

    ///@}
    ///@name Private Operations
    ///@{

    void SetNodalSensitivityVariableToZero(std::string const& rVariableName)
    {
        KRATOS_TRY;

        if (KratosComponents<Variable<double>>::Has(rVariableName) == true)
        {
            const Variable<double>& r_variable =
                KratosComponents<Variable<double>>::Get(rVariableName);

            VariableUtils().SetScalarVar(r_variable, r_variable.Zero(), mrModelPart.Nodes());
        }
        else if (KratosComponents<Variable<array_1d<double, 3>>>::Has(rVariableName) == true)
        {
            const Variable<array_1d<double, 3>>& r_variable =
                KratosComponents<Variable<array_1d<double, 3>>>::Get(rVariableName);

            VariableUtils().SetVectorVar(r_variable, r_variable.Zero(), mrModelPart.Nodes());
        }
        else
            KRATOS_ERROR << "Unsupported variable: " << rVariableName << "." << std::endl;

        KRATOS_CATCH("");
    }

    void SetElementSensitivityVariableToZero(std::string const& rVariableName)
    {
        KRATOS_TRY;

        auto& r_elements = mrModelPart.Elements();
        if (KratosComponents<Variable<double>>::Has(rVariableName) == true)
        {
            const Variable<double>& r_variable =
                KratosComponents<Variable<double>>::Get(rVariableName);
            #pragma omp parallel for
            for (int k = 0; k< static_cast<int> (r_elements.size()); ++k)
            {
                auto it = r_elements.begin() + k;
                it->SetValue(r_variable, r_variable.Zero());
            }
        }
        else if (KratosComponents<Variable<array_1d<double, 3>>>::Has(rVariableName) == true)
        {
            const Variable<array_1d<double, 3>>& r_variable =
                KratosComponents<Variable<array_1d<double, 3>>>::Get(rVariableName);
            #pragma omp parallel for
            for (int k = 0; k< static_cast<int> (r_elements.size()); ++k)
            {
                auto it = r_elements.begin() + k;
                it->SetValue(r_variable, r_variable.Zero());
            }
        }
        else
            KRATOS_ERROR << "Unsupported variable: " << rVariableName << "." << std::endl;

        KRATOS_CATCH("");
    }


    ///@}
};

///@} // Kratos Classes

} /* namespace Kratos.*/

#endif /* KRATOS_SENSITIVITY_BUILDER_H_INCLUDED defined */
