#ifndef TW_MODEL_H
#define TW_MODEL_H

#include <vector>

#include "twGeometry.h"
#include "twTransform.h"

namespace TwinkleGraphics
{
    class __TWCOMExport Model
    {
    public:
        typedef std::shared_ptr<Model> Ptr;

        Model();
        virtual ~Model();

        void AddGeometry(Geometry::Ptr geom);
        void RemoveGeometry(int index);
        void SetRootGeometry(Geometry::Ptr geom) { _rootGeometry = geom; }
        Geometry::Ptr GetGeometry(int index);
        Geometry::Ptr GetRootGeometry() { return _rootGeometry; }
        int32 GetGeometriesCount() { return _geometries.size(); }

        /**
         * just for test
         */
        void Render();

    protected:
        std::vector<Geometry::Ptr> _geometries;
        Geometry::Ptr _rootGeometry;
    };

    class SkeletonModel : public Model
    {
    public:
        typedef std::shared_ptr<SkeletonModel> Ptr;

        SkeletonModel();
        virtual ~SkeletonModel();

    private:
    };


} // namespace TwinkleGraphics
#endif
