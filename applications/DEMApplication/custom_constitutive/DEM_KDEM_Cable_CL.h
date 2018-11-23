
#if !defined(DEM_KDEM_Cable_CL_H_INCLUDED)
#define  DEM_KDEM_Cable_CL_H_INCLUDED

/* Project includes */
#include "DEM_continuum_constitutive_law.h"
#include "DEM_KDEM_CL.h"

namespace Kratos {

    class KRATOS_API(DEM_APPLICATION) DEM_KDEM_Cable : public DEM_KDEM {
    public:

        KRATOS_CLASS_POINTER_DEFINITION(DEM_KDEM_Cable);

        DEM_KDEM_Cable() {}

        ~DEM_KDEM_Cable() {}

        DEMContinuumConstitutiveLaw::Pointer Clone() const override;

        void SetConstitutiveLawInProperties(Properties::Pointer pProp, bool verbose = true) const override;

        void CalculateContactArea(double radius, double other_radius, double& calculation_area) override;

        void GetContactArea(const double radius, const double other_radius, const Vector& vector_of_initial_areas, const int neighbour_position, double& calculation_area) override;

    private:

        friend class Serializer;

        virtual void save(Serializer& rSerializer) const override{
            KRATOS_SERIALIZE_SAVE_BASE_CLASS(rSerializer, DEMContinuumConstitutiveLaw)
                    //rSerializer.save("MyMemberName",myMember);
        }

        virtual void load(Serializer& rSerializer) override{
            KRATOS_SERIALIZE_LOAD_BASE_CLASS(rSerializer, DEMContinuumConstitutiveLaw)
                    //rSerializer.load("MyMemberName",myMember);
        }

    };

} /* namespace Kratos.*/
#endif /* DEM_KDEM_Cable_H_INCLUDED  defined */
