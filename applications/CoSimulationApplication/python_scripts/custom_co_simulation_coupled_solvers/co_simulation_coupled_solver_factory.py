from __future__ import print_function, absolute_import, division  # makes these scripts backward compatible with python 2.6 and 2.7
"""
This is a map of the name of coupled solver to be specified in
JSON file and their python module (file) name. New coupled solvers should
be registered by an additional entry here.
eg : "name_in_JSON" : "python module(file) name"
"""
available_coupled_solvers = {
    "gauss_seidel"  : "gauss_seidel_iterative_strong_coupled_solver",
    "jacobi"        : "jacobi_iterative_strong_coupled_solver",
    "staggered"     : "staggered_loose_coupled_solver",
}


def CreateCoupledSolver(settings, solvers, level):
    """
    This function creates and returns the coupled solver used for CoSimulation
    One can register the a new coupled solver by adding them to the above map "available_coupled_solvers"
    """
    if (type(settings) != dict):
        raise Exception("Input is expected to be provided as a python dictionary")

    coupled_solver_type = settings["type"]

    if coupled_solver_type in available_coupled_solvers:
        coupled_solver_module = __import__(available_coupled_solvers[coupled_solver_type])
        return coupled_solver_module.Create(settings, solvers, level)
    else:
        err_msg  = 'The requested coupled solver "' + coupled_solver_type + '" is not available!\n'
        err_msg += 'Available coupled solvers are :\n'
        for available_coupled_solver in available_coupled_solvers:
            err_msg += "\t" + available_coupled_solver + "\n"
        raise NameError(err_msg)