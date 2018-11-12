//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Philipp Bucher, Jordi Cotela
//
// See Master-Thesis P.Bucher
// "Development and Implementation of a Parallel
//  Framework for Non-Matching Grid Mapping"

#if !defined(KRATOS_IGA_MAPPER_H_INCLUDED )
#define  KRATOS_IGA_MAPPER_H_INCLUDED

// System includes

// External includes

// Project includes
#include "mapper.h"


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

class IGAInterfaceInfo : public MapperInterfaceInfo
{
public:

    /// Default constructor.
    IGAInterfaceInfo() {}

    IGAInterfaceInfo(const CoordinatesArrayType& rCoordinates,
                                 const IndexType SourceLocalSystemIndex,
                                 const IndexType SourceRank)
        : MapperInterfaceInfo(rCoordinates, SourceLocalSystemIndex, SourceRank) {}

    MapperInterfaceInfo::Pointer Create() const override
    {
        return Kratos::make_shared<IGAInterfaceInfo>();
    }

    MapperInterfaceInfo::Pointer Create(const CoordinatesArrayType& rCoordinates,
                                        const IndexType SourceLocalSystemIndex,
                                        const IndexType SourceRank=0) const override
    {
        return Kratos::make_shared<IGAInterfaceInfo>(
            rCoordinates,
            SourceLocalSystemIndex,
            SourceRank);
    }

    void ProcessSearchResult(const InterfaceObject::Pointer& rpInterfaceObject,
                             const double NeighborDistance) override;

    void GetValue(int& rValue,
                  const InfoType ValueType=MapperInterfaceInfo::InfoType::Dummy) const override
    {
        rValue = mNearestNeighborId;
    }

    void GetValue(double& rValue,
                  const InfoType ValueType=MapperInterfaceInfo::InfoType::Dummy) const override
    {
        rValue = mNearestNeighborDistance;
    }

private:

    int mNearestNeighborId;
    double mNearestNeighborDistance = std::numeric_limits<double>::max();

    friend class Serializer;

    void save(Serializer& rSerializer) const override
    {
        KRATOS_SERIALIZE_SAVE_BASE_CLASS( rSerializer, MapperInterfaceInfo );
        rSerializer.save("NearestNeighborId", mNearestNeighborId);
        rSerializer.save("NearestNeighborDistance", mNearestNeighborDistance);
    }

    void load(Serializer& rSerializer) override
    {
        KRATOS_SERIALIZE_LOAD_BASE_CLASS( rSerializer, MapperInterfaceInfo );
        rSerializer.load("NearestNeighborId", mNearestNeighborId);
        rSerializer.load("NearestNeighborDistance", mNearestNeighborDistance);
    }
};

class IGALocalSystem : public MapperLocalSystem
{
public:

    IGALocalSystem() {}

    IGALocalSystem(NodePointerType pNode) : mpNode(pNode) {}

    MapperLocalSystemUniquePointer Create(NodePointerType pNode) const override
    {
        return Kratos::make_unique<IGALocalSystem>(pNode);
    }

    void CalculateAll(MatrixType& rLocalMappingMatrix,
                      EquationIdVectorType& rOriginIds,
                      EquationIdVectorType& rDestinationIds,
                      MapperLocalSystem::PairingStatus& rPairingStatus) const override;

    bool UseNodesAsBasis() const override { return true; }

    CoordinatesArrayType& Coordinates() const override
    {
        KRATOS_DEBUG_ERROR_IF_NOT(mpNode) << "Members are not intitialized!" << std::endl;
        return mpNode->Coordinates();
    }

    /// Turn back information as a string.
    std::string PairingInfo(const int EchoLevel, const int CommRank) const override;

private:
    NodePointerType mpNode;

};

/// Nearest Neighbor Mapper
/** This class implements the Nearest Neighbor Mapping technique.
* Each node on the destination side gets assigned is's closest neighbor on the other side of the interface.
* In the mapping phase every node gets assigned the value of it's neighbor
* For information abt the available echo_levels and the JSON default-parameters
* look into the class description of the MapperCommunicator
*/
template<class TSparseSpace, class TDenseSpace>
class IGAMapper : public Mapper<TSparseSpace, TDenseSpace>
{
public:

    ///@name Type Definitions
    ///@{

    ///@}
    ///@name Pointer Definitions
    /// Pointer definition of IGAMapper
    KRATOS_CLASS_POINTER_DEFINITION(IGAMapper);

    typedef Mapper<TSparseSpace, TDenseSpace> BaseType;
    typedef typename BaseType::MapperUniquePointerType MapperUniquePointerType;

    typedef typename BaseType::MapperInterfaceInfoUniquePointerType MapperInterfaceInfoUniquePointerType;
    typedef typename BaseType::MapperLocalSystemPointer MapperLocalSystemPointer;

    ///@}
    ///@name Life Cycle
    ///@{

    // Default constructor, needed for registration
    IGAMapper(ModelPart& rModelPartOrigin,
                          ModelPart& rModelPartDestination)
                          : Mapper<TSparseSpace, TDenseSpace>(
                              rModelPartOrigin,
                              rModelPartDestination) {}

    IGAMapper(ModelPart& rModelPartOrigin,
                          ModelPart& rModelPartDestination,
                          Parameters JsonParameters)
                          : Mapper<TSparseSpace, TDenseSpace>(
                              rModelPartOrigin,
                              rModelPartDestination,
                              JsonParameters)
    {
        // The Initialize function has to be called here bcs it internally calls virtual
        // functions that would not exist yet if it was called from the BaseClass!
        this->Initialize();
    }

    /// Destructor.
    virtual ~IGAMapper() { }

    ///@}
    ///@name Operators
    ///@{

    ///@}
    ///@name Operations
    ///@{

    MapperUniquePointerType Clone(ModelPart& rModelPartOrigin,
                                  ModelPart& rModelPartDestination,
                                  Parameters JsonParameters) override
    {
        return Kratos::make_unique<IGAMapper<TSparseSpace, TDenseSpace>>(
            rModelPartOrigin,
            rModelPartDestination,
            JsonParameters);
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

    /// Turn back information as a string.
    std::string Info() const override
    {
        return "IGAMapper";
    }

    /// Print information about this object.
    void PrintInfo(std::ostream& rOStream) const override
    {
        rOStream << "IGAMapper";
    }

    /// Print object's data.
    void PrintData(std::ostream& rOStream) const override
    {
    }

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

    MapperInterfaceInfoUniquePointerType GetMapperInterfaceInfo() const override
    {
        return Kratos::make_unique<IGAInterfaceInfo>();
    }

    MapperLocalSystemPointer GetMapperLocalSystem() const override
    {
        return Kratos::make_unique<IGALocalSystem>();
    }

    InterfaceObject::ConstructionType GetInterfaceObjectConstructionTypeOrigin() const override
    {
        return InterfaceObject::Node_Coords;
    }

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

    void CustomInitialize()
    {
        IGALocalSystem(IgaElement);

        InterfaceInfos = ConductSearching(); // one for each FEM-Element

        for info in InterfaceInfos) {
            IGALocalSystem the_local_system();

            the_local_system.AddInterfaceInfo(info);
        }




    }

    ///@}
    ///@name Private  Access
    ///@{

    ///@}
    ///@name Private Inquiry
    ///@{

    ///@}
    ///@name Un accessible methods
    ///@{

    /// Assignment operator.
    // IGAMapper& operator=(IGAMapper const& rOther) {}

    /// Copy constructor.
    //IGAMapper(IGAMapper const& rOther);

    ///@}

}; // Class IGAMapper

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{


/// input stream function


///@}

///@} addtogroup block
}  // namespace Kratos.

#endif // KRATOS_IGA_MAPPER_H_INCLUDED  defined