#ifndef TW_MODEL_H
#define TW_MODEL_H

#include <vector>

#include "twGeometry.h"
#include "twTransform.h"

namespace TwinkleGraphics {
class __TWCOMExport Model : public Object {
public:
  typedef std::shared_ptr<Model> Ptr;

  Model();
  virtual ~Model();
  virtual void SetValid(bool valid) override;

  void AddGeometry(GeometryPtr geom);
  void RemoveGeometry(int index);
  void SetRootGeometry(GeometryPtr geom) { _rootGeometry = geom; }
  GeometryPtr GetGeometry(int index);
  GeometryPtr GetRootGeometry() { return _rootGeometry; }
  int32 GetGeometriesCount() { return _geometries.size(); }

  /**
   * just for test
   */
  void Render();

protected:
  std::vector<GeometryPtr> _geometries;
  GeometryPtr _rootGeometry;
};

class SkeletonModel : public Model {
public:
  typedef std::shared_ptr<SkeletonModel> Ptr;

  SkeletonModel();
  virtual ~SkeletonModel();

private:
};

typedef Model::Ptr ModelPtr;

} // namespace TwinkleGraphics
#endif
