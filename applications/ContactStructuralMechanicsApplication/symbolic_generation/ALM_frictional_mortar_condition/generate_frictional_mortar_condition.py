from __future__ import print_function, absolute_import, division  # makes KratosMultiphysics backward compatible with python 2.6 and 2.7

from KratosMultiphysics import *
from KratosMultiphysics.ContactStructuralMechanicsApplication  import *

from sympy import *
from custom_sympy_fe_utilities import *
import operator

do_simplifications = False
mode = "c" #to output to a c++ file

impose_partion_of_unity = False

def convert_chain_int_int(list_slip_stick):
    value = 0
    count = 0
    for slip_stick in list_slip_stick:
        value += slip_stick * 3**count
        count += 1
    return value

# Debug
#dim_combinations = [2]
#nnodes_combinations = [2]
#normal_combs = 1

dim_combinations = [2,3,3]
nnodes_combinations = [2,3,4]
normal_combs = 2

def real_norm(input):

    output = 0

    for i in range(input.shape[1]):
        output += input[i]**2

    output = real_root(output, 2)

    return output

lhs_template_begin_string = "\n/***********************************************************************************/\n/***********************************************************************************/\n\ntemplate<>\nvoid AugmentedLagrangianMethodFrictionalMortarContactCondition<TDim,TNumNodes, TNormalVariation>::CalculateLocalLHS(\n    Matrix& rLocalLHS,\n    const MortarConditionMatrices& rMortarConditionMatrices,\n    const DerivativeDataType& rDerivativeData,\n    const IndexType rActiveInactive,\n    const ProcessInfo& rCurrentProcessInfo\n    )\n{\n    // Initialize\n    for (std::size_t i = 0; i < MatrixSize; ++i)\n        for (std::size_t j = 0; j < MatrixSize; ++j)\n            rLocalLHS(i, j) = 0.0;\n\n    // The geometry of the condition\n    GeometryType& geometry = this->GetGeometry();\n\n    // Initialize values\n    const BoundedMatrix<double, TNumNodes, TDim>& u1 = rDerivativeData.u1;\n    const BoundedMatrix<double, TNumNodes, TDim>& u1old = rDerivativeData.u1old;\n    const BoundedMatrix<double, TNumNodes, TDim>& u2 = rDerivativeData.u2;\n    const BoundedMatrix<double, TNumNodes, TDim>& u2old = rDerivativeData.u2old;\n    const BoundedMatrix<double, TNumNodes, TDim>& X1 = rDerivativeData.X1;\n    const BoundedMatrix<double, TNumNodes, TDim>& X2 = rDerivativeData.X2;\n    \n    const BoundedMatrix<double, TNumNodes, TDim> LM = MortarUtilities::GetVariableMatrix<TDim,TNumNodes>(geometry, VECTOR_LAGRANGE_MULTIPLIER, 0);\n    \n    // The normal and tangent vectors\n    const BoundedMatrix<double, TNumNodes, TDim>& NormalSlave = rDerivativeData.NormalSlave;\n    const BoundedMatrix<double, TNumNodes, TDim> TangentSlave = MortarUtilities::ComputeTangentMatrix<TNumNodes,TDim>(geometry);\n\n    // The ALM parameters\n    const array_1d<double, TNumNodes> DynamicFactor = MortarUtilities::GetVariableVector<TNumNodes>(geometry, DYNAMIC_FACTOR);\n    const double ScaleFactor = rDerivativeData.ScaleFactor;\n    const array_1d<double, TNumNodes>& PenaltyParameter = rDerivativeData.PenaltyParameter;\n    const double TangentFactor = rDerivativeData.TangentFactor;\n    \n    // Mortar operators\n    const BoundedMatrix<double, TNumNodes, TNumNodes>& MOperator = rMortarConditionMatrices.MOperator;\n    const BoundedMatrix<double, TNumNodes, TNumNodes>& DOperator = rMortarConditionMatrices.DOperator;\n    const BoundedMatrix<double, TNumNodes, TNumNodes>& StandardMOperator = mCurrentMortarOperators.MOperator;\n    const BoundedMatrix<double, TNumNodes, TNumNodes>& StandardDOperator = mCurrentMortarOperators.DOperator;\n    const BoundedMatrix<double, TNumNodes, TNumNodes>& StandardMOperatorold = mPreviousMortarOperators.MOperator;\n    const BoundedMatrix<double, TNumNodes, TNumNodes>& StandardDOperatorold = mPreviousMortarOperators.DOperator;\n\n    // Mortar operators derivatives\n    const array_1d<BoundedMatrix<double, TNumNodes, TNumNodes>, SIZEDERIVATIVES2>& DeltaMOperator = rMortarConditionMatrices.DeltaMOperator;\n    const array_1d<BoundedMatrix<double, TNumNodes, TNumNodes>, SIZEDERIVATIVES2>& DeltaDOperator = rMortarConditionMatrices.DeltaDOperator;\n\n    // We get the friction coefficient\n    const array_1d<double, TNumNodes> mu = GetFrictionCoefficient();\n\n//    // The delta time\n//    const double delta_time = rCurrentProcessInfo[DELTA_TIME];\n"

lhs_template_end_string = "}\n"

rhs_template_begin_string = "\n/***********************************************************************************/\n/***********************************************************************************/\n\ntemplate<>\nvoid AugmentedLagrangianMethodFrictionalMortarContactCondition<TDim,TNumNodes, TNormalVariation>::CalculateLocalRHS(\n    Vector& rLocalRHS,\n    const MortarConditionMatrices& rMortarConditionMatrices,\n    const DerivativeDataType& rDerivativeData,\n    const IndexType rActiveInactive,\n    const ProcessInfo& rCurrentProcessInfo\n    )\n{\n    // Initialize\n    for (std::size_t i = 0; i < MatrixSize; ++i)\n        rLocalRHS[i] = 0.0;\n\n    // The geometry of the condition\n    GeometryType& geometry = this->GetGeometry();\n\n    // Initialize values\n    const BoundedMatrix<double, TNumNodes, TDim>& u1 = rDerivativeData.u1;\n    const BoundedMatrix<double, TNumNodes, TDim>& u1old = rDerivativeData.u1old;\n    const BoundedMatrix<double, TNumNodes, TDim>& u2 = rDerivativeData.u2;\n    const BoundedMatrix<double, TNumNodes, TDim>& u2old = rDerivativeData.u2old;\n    const BoundedMatrix<double, TNumNodes, TDim>& X1 = rDerivativeData.X1;\n    const BoundedMatrix<double, TNumNodes, TDim>& X2 = rDerivativeData.X2;\n    \n    const BoundedMatrix<double, TNumNodes, TDim> LM = MortarUtilities::GetVariableMatrix<TDim,TNumNodes>(geometry, VECTOR_LAGRANGE_MULTIPLIER, 0);\n    \n    // The normal and tangent vectors\n    const BoundedMatrix<double, TNumNodes, TDim>& NormalSlave = rDerivativeData.NormalSlave;\n    const BoundedMatrix<double, TNumNodes, TDim> TangentSlave = MortarUtilities::ComputeTangentMatrix<TNumNodes,TDim>(geometry);\n\n    // The ALM parameters\n    const array_1d<double, TNumNodes> DynamicFactor = MortarUtilities::GetVariableVector<TNumNodes>(geometry, DYNAMIC_FACTOR);\n    const double ScaleFactor = rDerivativeData.ScaleFactor;\n    const array_1d<double, TNumNodes>& PenaltyParameter = rDerivativeData.PenaltyParameter;\n    const double TangentFactor = rDerivativeData.TangentFactor;\n    \n    // Mortar operators\n    const BoundedMatrix<double, TNumNodes, TNumNodes>& MOperator = rMortarConditionMatrices.MOperator;\n    const BoundedMatrix<double, TNumNodes, TNumNodes>& DOperator = rMortarConditionMatrices.DOperator;\n    const BoundedMatrix<double, TNumNodes, TNumNodes>& StandardMOperator = mCurrentMortarOperators.MOperator;\n    const BoundedMatrix<double, TNumNodes, TNumNodes>& StandardDOperator = mCurrentMortarOperators.DOperator;\n    const BoundedMatrix<double, TNumNodes, TNumNodes>& StandardMOperatorold = mPreviousMortarOperators.MOperator;\n    const BoundedMatrix<double, TNumNodes, TNumNodes>& StandardDOperatorold = mPreviousMortarOperators.DOperator;\n\n    // We get the friction coefficient\n    const array_1d<double, TNumNodes> mu = GetFrictionCoefficient();\n\n//    // The delta time\n//    const double delta_time = rCurrentProcessInfo[DELTA_TIME];\n"

rhs_template_end_string = "}\n"

# We track the output each time
output_count = 0
total_combs = normal_combs * len(nnodes_combinations)

for normalvar in range(2):

    if normalvar == 0:
        normalvarstring = "false"
    else:
        normalvarstring = "true"

    if normalvar == 1:
        lhs_template_begin_string += "   const array_1d<BoundedMatrix<double, TNumNodes, TDim>,  (TNumNodes * TDim)> DeltaNormalSlave = rDerivativeData.DeltaNormalSlave;\n\n"

    for dim, nnodes in zip(dim_combinations, nnodes_combinations):

        # Update counter and calculate dof
        lhs_string = ""
        rhs_string = ""
        output_count += 1
        number_dof = dim * (3 * nnodes)

        # Defining the unknowns
        u1 = DefineMatrix('u1', nnodes, dim) # u1(i,j) is the current displacement of node i component j at domain 1
        u2 = DefineMatrix('u2', nnodes, dim) # u2(i,j) is the current displacement of node i component j at domain 2
        u1old = DefineMatrix('u1old', nnodes, dim) # u1(i,j) is the previous displacement of node i component j at domain 1
        u2old = DefineMatrix('u2old', nnodes, dim) # u2(i,j) is the previous displacement of node i component j at domain 2
        LM = DefineMatrix('LM', nnodes, dim)
        # Normal and tangets of the slave
        NormalSlave = DefineMatrix('NormalSlave', nnodes, dim)

        # The resultant tangent
        TangentSlave = DefineMatrix('TangentSlave', nnodes, dim)

        # Define test functions
        w1 = DefineMatrix('w1',nnodes,dim)
        w2 = DefineMatrix('w2',nnodes,dim)
        wLM = DefineMatrix('wLM',nnodes,dim)

        # Defining normal and tangent components
        LMNormal = DefineVector('LMNormal', nnodes)
        wLMNormal = DefineVector('wLMNormal', nnodes)

        # The resultant tangent LM
        LMTangent = DefineMatrix('LMTangent', nnodes, dim)
        wLMTangent = DefineMatrix('wLMTangent', nnodes, dim)

        for node in range(nnodes):
            LMNormal[node] = LM.row(node).dot(NormalSlave.row(node))
            wLMNormal[node] = wLM.row(node).dot(NormalSlave.row(node))

            #auxLMTangent = (LM.row(node)).dot(TangentSlave.row(node)) * TangentSlave.row(node)
            #auxwLMTangent = (wLM.row(node)).dot(TangentSlave.row(node)) * TangentSlave.row(node)

            # We calculate the LM tangent resultant
            for idim in range(dim):
                #LMTangent[node,idim] = auxLMTangent[idim]
                #wLMTangent[node,idim] = auxwLMTangent[idim]
                LMTangent[node,idim] = LM[node,idim] - LMNormal[node] * NormalSlave[node, idim]
                wLMTangent[node,idim] = wLM[node,idim] - wLMNormal[node] * NormalSlave[node, idim]

            # Add implicit definition
            aux_tangent_slave = LMTangent.row(node)/real_norm(LMTangent.row(node))
            for idim in range(dim):
                TangentSlave[node,idim] = aux_tangent_slave[idim]

        # Defining additional variables
        NormalGap = DefineVector('NormalGap', nnodes)
        NormalwGap = DefineVector('NormalwGap', nnodes)
        GapTimeDerivative = DefineMatrix('GapTimeDerivative', nnodes, dim)
        TangentSlip = DefineMatrix('TangentSlip', nnodes, dim)
        TangentwSlip = DefineMatrix('TangentwSlip', nnodes, dim)
        DOperator = DefineMatrix('DOperator', nnodes, nnodes)
        MOperator = DefineMatrix('MOperator', nnodes, nnodes)
        StandardDOperator = DefineMatrix('StandardDOperator',nnodes,nnodes)
        StandardMOperator = DefineMatrix('StandardMOperator',nnodes,nnodes)
        StandardDOperatorold = DefineMatrix('StandardDOperatorold',nnodes,nnodes)
        StandardMOperatorold = DefineMatrix('StandardMOperatorold',nnodes,nnodes)

        # Define other parameters
        X1 = DefineMatrix('X1',nnodes,dim)
        X2 = DefineMatrix('X2',nnodes,dim)
        x1 = X1 + u1
        x2 = X2 + u2
        x1old = X1 + u1old
        x2old = X2 + u2old

        #Define other symbols
        mu  = DefineVector('mu',nnodes)
        DynamicFactor  = DefineVector('DynamicFactor',nnodes)
        PenaltyParameter  = DefineVector('PenaltyParameter',nnodes)
        delta_time = Symbol('delta_time', positive=True)
        ScaleFactor = Symbol('ScaleFactor', positive=True)
        TangentFactor = Symbol('TangentFactor', positive=True)

        # Define variables list for later derivation
        u1_var = []
        u2_var = []
        LM_var = []
        CreateVariableMatrixList(u1_var, u1)
        CreateVariableMatrixList(u2_var, u2)
        u12_var=u1_var.copy()
        u1_LM_var=u1_var.copy()
        CreateVariableMatrixList(u12_var, u2)
        CreateVariableMatrixList(LM_var, LM)
        CreateVariableMatrixList(u1_LM_var, LM)
        all_var=u12_var.copy()
        CreateVariableMatrixList(all_var, LM)

        # Force the variables to be dependendant of the DOF
        if normalvar == 1:
            NormalSlave = DefineDofDependencyMatrix(NormalSlave, u1_var)
        DOperator = DefineDofDependencyMatrix(DOperator, u12_var) # If you consider Gitterle you need to keep the old operators
        MOperator = DefineDofDependencyMatrix(MOperator, u12_var)

        # Defining the normal NormalGap and tangent slip
        Du1Mu2 = DOperator * u1 - MOperator * u2
        Dx1Mx2 = DOperator * x1 - MOperator * x2
        #DeltaDu1DeltaMu2 = (DOperator - StandardDOperatorold)/delta_time * u1 - (MOperator - StandardMOperatorold)/delta_time * u2
        DeltaDx1DeltaMx2 = (DOperator - StandardDOperatorold)/delta_time * x1 - (MOperator - StandardMOperatorold)/delta_time * x2
        #DeltaDx1DeltaMx2 = (StandardDOperator - StandardDOperatorold)/delta_time * x1old - (StandardMOperator - StandardMOperatorold)/delta_time * x2old
        #DeltaDx1DeltaMx2 = (StandardDOperator - StandardDOperatorold)/delta_time * x1 - (StandardMOperator - StandardMOperatorold)/delta_time * x2
        #DDeltax1MDeltax2 = DOperator * x1old/delta_time - MOperator * x2old/delta_time # Cardona, doesn't work
        DDeltax1MDeltax2 = DOperator * u1/delta_time - MOperator * u2/delta_time
        #DDeltax1MDeltax2 = StandardDOperator * u1/delta_time - StandardMOperator * u2/delta_time
        #DDeltax1MDeltax2 = DOperator * (x1 - x1old)/delta_time - MOperator * (x2 - x2old)/delta_time
        Dw1Mw2 = DOperator * w1 - MOperator * w2
        for node in range(nnodes):
            NormalGap[node] = - Dx1Mx2.row(node).dot(NormalSlave.row(node))
            NormalwGap[node] = - Dw1Mw2.row(node).dot(NormalSlave.row(node))
            #objective_gap_time_derivative = DeltaDu1DeltaMu2.row(node)
            objective_gap_time_derivative = DeltaDx1DeltaMx2.row(node)
            #objective_gap_time_derivative = DDeltax1MDeltax2.row(node)
            #objective_gap_time_derivative = (DDeltax1MDeltax2.row(node) - (DDeltax1MDeltax2.row(node) + DeltaDx1DeltaMx2.row(node)))
            gap_time_derivative = - (DDeltax1MDeltax2.row(node))
            #gap_time_derivative = - (DDeltax1MDeltax2.row(node) + DeltaDx1DeltaMx2.row(node))
            #auxTangentSlip = delta_time * (DDeltax1MDeltax2.row(node)).dot(TangentSlave.row(node)) * TangentSlave.row(node)
            #auxTangentSlip = delta_time * objective_gap_time_derivative.dot(TangentSlave.row(node)) * TangentSlave.row(node)
            auxTangentwSlip = (Dw1Mw2.row(node)).dot(TangentSlave.row(node)) * TangentSlave.row(node)
            #auxTangentSlip = - (Dx1Mx2.row(node) - Dx1Mx2.row(node).dot(NormalSlave.row(node)) * NormalSlave.row(node))
            auxTangentSlip = delta_time * (objective_gap_time_derivative - objective_gap_time_derivative.dot(NormalSlave.row(node)) * NormalSlave.row(node))
            #auxTangentSlip = delta_time * (DDeltax1MDeltax2.row(node) - DDeltax1MDeltax2.row(node).dot(NormalSlave.row(node)) * NormalSlave.row(node))
            #auxTangentwSlip = Dw1Mw2.row(node) + NormalwGap[node] * NormalSlave.row(node)
            for idim in range(dim):
                TangentSlip[node, idim] = delta_time * gap_time_derivative[idim]
                #TangentSlip[node, idim] = - auxTangentSlip[idim]
                TangentwSlip[node, idim] = - auxTangentwSlip[idim]
                GapTimeDerivative[node, idim] = gap_time_derivative[idim]

        # Define dofs & test function vector
        dofs = Matrix( zeros(number_dof, 1) )
        testfunc = Matrix( zeros(number_dof, 1) )
        count = 0
        for i in range(0,nnodes):
            for k in range(0,dim):
                dofs[count] = u2[i,k]
                testfunc[count] = w2[i,k]
                count+=1
        for i in range(0,nnodes):
            for k in range(0,dim):
                dofs[count] = u1[i,k]
                testfunc[count] = w1[i,k]
                count+=1
        for i in range(0,nnodes):
            for k in range(0,dim):
                dofs[count] = LM[i,k]
                testfunc[count] = wLM[i,k]
                count+=1
        print("dofs = ",dofs)
        print("testfunc = ",testfunc)

        #############################################################################
        #############################################################################
        ########################## FUNCTIONAL DEFINITION ############################
        #############################################################################
        #############################################################################

        # Compute galerkin functional # NOTE: Maybe you can define a different penalty and scale factor in the tangent direction NOTE: This is for Galerkin functional
        lhs_string += lhs_template_begin_string
        rhs_string += rhs_template_begin_string
        for node in range(nnodes):
            for slip in range(3):
            #for slip in range(4):
                rv_galerkin = 0
                if (slip == 0): # Inactive
                    rv_galerkin -= ScaleFactor**2 / PenaltyParameter[node] * LMNormal[node] * wLMNormal[node]
                    rv_galerkin -= ScaleFactor**2 / (PenaltyParameter[node] * TangentFactor) * (LMTangent.row(node)).dot(wLMTangent.row(node))
                else:
                    # Normal contact
                    #rv_galerkin -= ScaleFactor * NormalGap[node] * wLMNormal[node]
                    augmented_normal_lm = ScaleFactor * LMNormal[node] + PenaltyParameter[node] * NormalGap[node]
                    #rv_galerkin += DynamicFactor[node] * augmented_normal_lm * NormalwGap[node]

                    # Frictional contact
                    #augmented_tangent_lm = ScaleFactor * LMTangent.row(node) + TangentFactor * PenaltyParameter[node] * TangentSlip.row(node)
                    #rv_galerkin += DynamicFactor[node] * (augmented_tangent_lm).dot(TangentwSlip.row(node))
                    #augmented_lm = (ScaleFactor * LM.row(node) + PenaltyParameter[node] * NormalGap[node] * NormalSlave.row(node))
                    #augmented_lm = (ScaleFactor * LM.row(node))
                    #augmented_lm = (ScaleFactor * LM.row(node) + PenaltyParameter[node] * NormalGap[node] * NormalSlave.row(node))
                    #augmented_lm = (ScaleFactor * LM.row(node) + PenaltyParameter[node] * NormalGap[node] * NormalSlave.row(node) + TangentFactor * PenaltyParameter[node] * TangentSlip.row(node))
                    if (slip == 1):
                        augmented_tangent_lm = mu[node] * augmented_normal_lm * TangentSlave.row(node)
                        augmented_lm = augmented_normal_lm * NormalSlave.row(node) - augmented_tangent_lm
                    else:
                        augmented_tangent_lm = ScaleFactor * LMTangent.row(node) + TangentFactor * PenaltyParameter[node] * TangentSlip.row(node)
                        augmented_lm = augmented_normal_lm * NormalSlave.row(node) + augmented_tangent_lm

                    rv_galerkin += DynamicFactor[node] * (augmented_lm).dot(Dw1Mw2.row(node))
                    if (slip == 1): # Slip
                        # Normal contact
                        rv_galerkin -= ScaleFactor * NormalGap[node] * wLMNormal[node]
                        # Frictional contact
                        #modified_augmented_tangent_lm = real_norm(augmented_tangent_lm) * ScaleFactor * LMTangent.row(node) - mu[node] * augmented_normal_lm * augmented_tangent_lm
                        #rv_galerkin -= (1.0 / (PenaltyParameter[node] * TangentFactor)) * modified_augmented_tangent_lm.dot(wLMTangent.row(node))
                        #modified_augmented_tangent_lm = ScaleFactor * LMTangent.row(node) + mu[node] * augmented_normal_lm * TangentSlave.row(node)
                        modified_augmented_tangent_lm = ScaleFactor * LMTangent.row(node) + augmented_tangent_lm
                        rv_galerkin -= (ScaleFactor / (PenaltyParameter[node] * TangentFactor)) * modified_augmented_tangent_lm.dot(wLMTangent.row(node))
                        #rv_galerkin -= ScaleFactor**2/PenaltyParameter[node] * (wLMTangent.row(node).dot(LMTangent.row(node)))
                    else: # Stick
                        # Normal contact
                        rv_galerkin -= ScaleFactor * NormalGap[node] * wLMNormal[node]
                        # Frictional contact
                        #modified_augmented_tangent_lm = mu[node] * augmented_normal_lm * TangentFactor * PenaltyParameter[node] * TangentSlip.row(node)
                        #rv_galerkin += (modified_augmented_tangent_lm).dot(wLMTangent.row(node))
                        rv_galerkin += ScaleFactor * (TangentSlip.row(node)).dot(wLMTangent.row(node))
                        #rv_galerkin -= ScaleFactor**2 / PenaltyParameter[node] * (GapTimeDerivative.row(node)).dot(wLM.row(node)) # Auxiliar term to ensure zero gap derivative
                        #rv_galerkin += ScaleFactor * (delta_time * GapTimeDerivative.row(node)).dot(wLM.row(node))
                        #total_gap = - NormalGap[node] * NormalSlave.row(node) + TangentSlip.row(node)
                        #rv_galerkin += ScaleFactor * total_gap.dot(wLM.row(node))

                if(do_simplifications):
                    rv_galerkin = simplify(rv_galerkin)

                #############################################################################
                # Complete functional
                rv = Matrix(zeros(1, 1))
                rv[0,0] = rv_galerkin

                rhs,lhs = Compute_RHS_and_LHS(rv.copy(), testfunc, dofs, False)
                print("LHS= ", lhs.shape)
                print("RHS= ", rhs.shape)
                print("LHS and RHS have been created!")

                initial_tabs = 1
                lhs_out = OutputMatrix_CollectingFactorsNonZero(lhs, "lhs", mode, initial_tabs, number_dof)
                rhs_out = OutputVector_CollectingFactorsNonZero(rhs, "rhs", mode, initial_tabs, number_dof)
                print("Substitution strings are ready....")

                if (slip == 0):
                    lhs_string += "    \n    // NODE " + str(node) + "\n"
                    lhs_string += "    if (geometry["+str(node)+"].IsNot(ACTIVE)) { // INACTIVE\n    "
                elif (slip == 1):
                    lhs_string += "} else if (geometry["+str(node)+"].Is(SLIP)) { // ACTIVE-SLIP\n    "
                else:
                    lhs_string += "} else { // ACTIVE-STICK\n    "
                lhs_string += lhs_out.replace("\n","\n    ")
                if (slip == 2):
                    lhs_string += "}\n"

                if (slip == 0):
                    rhs_string += "    \n    // NODE " + str(node) + "\n"
                    rhs_string += "    if (geometry["+str(node)+"].IsNot(ACTIVE)) { // INACTIVE\n    "
                elif (slip == 1):
                    rhs_string += "} else if (geometry["+str(node)+"].Is(SLIP)) { // ACTIVE-SLIP\n    "
                else:
                    rhs_string += "} else { // ACTIVE-STICK\n    "
                rhs_string += rhs_out.replace("\n","\n    ")
                if (slip == 2):
                    rhs_string += "}\n"

        lhs_string += lhs_template_end_string
        rhs_string += rhs_template_end_string

        lhs_string = lhs_string.replace("TDim", str(dim))
        lhs_string = lhs_string.replace("TNumNodes", str(nnodes))
        lhs_string = lhs_string.replace("MatrixSize", str(lhs.shape[0]))
        lhs_string = lhs_string.replace("TNormalVariation", normalvarstring)
        lhs_string = lhs_string.replace("SIZEDERIVATIVES2", str(2 * (nnodes * dim)))

        rhs_string = rhs_string.replace("TDim", str(dim))
        rhs_string = rhs_string.replace("TNumNodes", str(nnodes))
        rhs_string = rhs_string.replace("TNormalVariation", normalvarstring)
        rhs_string = rhs_string.replace("MatrixSize", str(lhs.shape[0]))

        ##############################################################################
        ##############################################################################
        ##################### DEFINE VARIABLES AND DERIVATIVES #######################
        ##############################################################################
        ##############################################################################

        var_strings = []
        var_strings_subs = []
        var_strings_aux_subs = []
        der_var_strings = []
        der_var_list = []
        der_var_used_index = []

        if normalvar == 1:
            var_strings, var_strings_subs, var_strings_aux_subs, der_var_strings, der_var_list = DefineVariableLists(NormalSlave, "NormalSlave", "NormalSlave", u1_var, var_strings, var_strings_subs, var_strings_aux_subs, der_var_strings, der_var_list, "matrix")
        var_strings, var_strings_subs, var_strings_aux_subs, der_var_strings, der_var_list = DefineVariableLists(DOperator, "DOperator", "DOperator", u12_var, var_strings, var_strings_subs, var_strings_aux_subs, der_var_strings, der_var_list, "matrix")
        var_strings, var_strings_subs, var_strings_aux_subs, der_var_strings, der_var_list = DefineVariableLists(MOperator, "MOperator", "MOperator", u12_var, var_strings, var_strings_subs, var_strings_aux_subs, der_var_strings, der_var_list, "matrix")

        #############################################################################
        ############################### SUBSTITUTION ################################
        #############################################################################

        # Replace all
        lhs_string = lhs_string.replace("//subsvar_", "")
        rhs_string = rhs_string.replace("//subsvar_", "")

        for index in range(len(der_var_strings)):
            lhs_string = lhs_string.replace(der_var_strings[index], der_var_list[index])
            rhs_string = rhs_string.replace(der_var_strings[index], der_var_list[index])

        for index in range(len(var_strings)):
            lhs_string = lhs_string.replace(var_strings[index], var_strings_subs[index])
            rhs_string = rhs_string.replace(var_strings[index], var_strings_subs[index])

        lhs_string = SubstituteIndex(lhs_string, mode, number_dof)
        rhs_string = SubstituteIndex(rhs_string, mode, number_dof)
        lhs_string = lhs_string.replace("array[1]d", "array_1d") # Repair the definition
        rhs_string = rhs_string.replace("array[1]d", "array_1d") # Repair the definition

        #############################################################################
        ############################### SIMPLIFICATION ##############################
        #############################################################################

        lhs_string = lhs_string.replace("pow(", "std::pow(")
        lhs_string = lhs_string.replace("sqrt(", "std::sqrt(")
        rhs_string = rhs_string.replace("pow(", "std::pow(")
        rhs_string = rhs_string.replace("sqrt(", "std::sqrt(")
        lhs_string = lhs_string.replace("lhs(", "rLocalLHS(")
        rhs_string = rhs_string.replace("rhs[", "rLocalRHS[")

        for dof in reversed(range(len(u1_var))):
            lhs_string = lhs_string.replace("DeltaNormalSlave"+str(dof), "DeltaNormalSlave["+str(dof)+"]")
        for dof in reversed(range(len(u12_var))):
            lhs_string = lhs_string.replace("DeltaDOperator"+str(dof), "DeltaDOperator["+str(dof)+"]")
        for dof in reversed(range(len(u12_var))):
            lhs_string = lhs_string.replace("DeltaMOperator"+str(dof), "DeltaMOperator["+str(dof)+"]")

        #############################################################################
        ################################# FINAL SAVING ##############################
        #############################################################################

        if (output_count == 1):
            input = open("ALM_frictional_mortar_contact_condition_template.cpp",'r').read()
        else:
            input = open("ALM_frictional_mortar_contact_condition.cpp",'r').read()
        if (output_count < total_combs):
            lhs_string += "// replace_lhs"
            rhs_string += "// replace_rhs"
        outputstring = input.replace("// replace_lhs", lhs_string)
        outputstring = outputstring.replace("// replace_rhs", rhs_string)
        output = open("ALM_frictional_mortar_contact_condition.cpp",'w')
        output.write(outputstring)
        output.close()

print("Strings have been replaced...")

print("Process Finished..................")
