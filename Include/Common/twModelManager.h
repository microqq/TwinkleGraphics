#ifndef TW_MODELMANAGER_H
#define TW_MODELMANAGER_H

#include "twModelReader.h"

namespace TwinkleGraphics
{

    class __SINGLETONExport ModelManager : public IUpdatable, public INonCopyable
    {
    public:
        virtual ~ModelManager() {}
        virtual void Update() override {} 

        Model::Ptr ReadModel(const char *filename);
        void ReadModelAsync(const char* filename, ShaderOption* option);

    private:
        explicit ModelManager() {}

        friend class Singleton<ModelManager>;
    };    


#ifdef __cplusplus
    extern "C"
    {
#endif
        __SINGLETONExport ModelManager& ModelMgrInstance();
#ifdef __cplusplus
    }
#endif

    typedef Singleton<ModelManager> ModelManagerInst;

} // namespace TwinkleGraphics



#endif