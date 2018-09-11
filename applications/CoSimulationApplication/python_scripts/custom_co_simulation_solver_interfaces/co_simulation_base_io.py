from __future__ import print_function, absolute_import, division  # makes these scripts backward compatible with python 2.6 and 2.7

class CoSimulationBaseIO(object):
    ## The constructor
    #
    #  @param self            The object pointer.
    def __init__(self, solver):
        self.lvl = level
        self.echo_level = 0

    ## ImportData :  used to import data from other clients
    #
    #  @param self            The object pointer.
    #  @param data_config     python dictionary : configuration of the data to be imported.
    #                                             data will be imported into this dictionary
    #  @param from_client     python object : The client from which data is to be imported.
    def ImportData(self, data_config, from_client):
        pass

    ## ImportMesh :  used to import mesh from other clients
    #
    #  @param self            The object pointer.
    #  @param mesh_conig      python dictionary : configuration of the mesh to be imported.
    #                                             mesh will be imported into this dictionary.
    #  @param from_client     python object : The client from which mesh is to be imported.
    def ImportMesh(self, mesh_conig, from_client):
        pass

    ## ExportData :  used to export data to other clients
    #
    #  @param self            The object pointer.
    #  @param data_config     python dictionary : configuration of the mesh to be exported.
    #                                             also contains the data to export.
    #  @param to_client       python object : The client to which mesh is to be exported.
    def ExportData(self, data_config, to_client):
        pass

    ## ExportMesh :  used to export mesh to other clients
    #
    #  @param self            The object pointer.
    #  @param mesh_conig      python dictionary : configuration of the mesh to be exported.
    #                                             also contains the mesh data to export.
    #  @param to_client       python object : The client to which mesh is to be exported.
    def ExportMesh(self, mesh_conig, to_client):
        pass

    ## Sets the echo level of for this IO object. Used for output of information during CoSimulation
    #
    #  @param self            The object pointer.
    #  @param level           int : echo level
    def SetEchoLevel(self, level):
        self.echo_level = level

    ## Prints the information about the current IO. The derived classes should implement it.
    #
    #  @param self            The object pointer.
    def PrintInfo(self):
        print("!!!WARNING!!! Calling PrintInfo from base IO class!")

    ## Check : Checks the current IO against faulty settings. Derived classes may implement this function.
    #
    #  @param self            The object pointer.
    def Check(self):
        print("!!!WARNING!!! Calling Check from base IO class!")
