
#include <sstream>
#include <regex>
#include <array>

#include "twShader.h"
#include "twConsoleLog.h"

namespace TwinkleGraphics
{

    ShaderManager::ShaderManager()
    {
    }

    ShaderManager::~ShaderManager()
    {
    }

    Shader::Ptr ShaderManager::ReadShader(ShaderReadInfo &shaderInfo)
    {
        ResourceManagerInst resMgr;
        ReadResult<Shader::Ptr> result = resMgr->Read<ShaderReader, Shader::Ptr>(shaderInfo.filename.c_str(), nullptr, shaderInfo);
        Shader::Ptr sharedShader = result.GetSharedObject();

        return sharedShader;
    }

    ShaderProgram::Ptr ShaderManager::ReadShaders(ShaderReadInfo shaderInfos[], int32 num)
    {
        //Todo: read program from cache first

        //read shader source
        bool readyLink = true;
        Shader::Ptr* shaders = new Shader::Ptr[num];
        for (int i = 0; i < num; i++)
        {
            Shader::Ptr shader = ReadShader(shaderInfos[i]);
            if(shader != nullptr)
            {
                shaders[i] = shader;
                readyLink &= shaders[i]->Compiled();
            }
        }

        //if cache not found, create new program
        ShaderProgram::Ptr program = std::make_shared<ShaderProgram>(shaders, num);
        if (readyLink)
        {
            program->Link();
        }
        SAFE_DEL_ARR(shaders);

        return program;
    }

    Shader::Shader(ShaderType type, ShaderSource::Ptr source)
        : Object()
        , _res()
        , _compiled(false)
    {
        _res.type = (uint32)type;
        _res.id = glCreateShader(_res.type);

        SetShaderSource(source);

#ifdef _DEBUG
        GLenum error = glGetError();
        const GLubyte *error_str = glGetString(error);
#endif
    }

//     /**
//  * @brief Construct a new Shader:: Shader object
//  * 
//  * @param type 
//  */
//     Shader::Shader(ShaderType type, const char *source)
//         : Object()
//         , _res()
//         , _compiled(false)
//     {
//         _res.type = (uint32)type;
//         _res.id = glCreateShader(_res.type);

//         glShaderSource(_res.id, 1, &source, NULL);
//         glCompileShader(_res.id);

//         // test
//         {
//             _source = std::make_shared<ShaderSource>();
//             _source->content = std::string(source);
//             _source->filename = "";

//             const char* macros[] = {"#define VERTCOLOR\n", "#define VERTNORMAL\n"};
//             SetDefineMacros(macros, 2);
//         }


// #ifdef _DEBUG
//         GLenum error = glGetError();
//         const GLubyte *error_str = glGetString(error);
// #endif

//         _compiled = true;
//         _setupCompile = true;
//     }

    Shader::Shader(const Shader &copy)
        : _res(copy._res)
    {
    }

    Shader::~Shader()
    {
        Console::LogInfo("Shader: Shader ", _res.id, "(hash: ", _res.hash, ") deconstruct.\n");

        if (_res.id != GL_NONE)
        {
            glDeleteShader(_res.id);
        }

        for(auto includeSource : _includeSouces)
        {
            if(includeSource != nullptr)
            {
                glDeleteNamedStringARB(includeSource->filename.length(), includeSource->filename.c_str());
            }
        }        

        _includeSouces.clear();
        _source = nullptr;
    }

    void Shader::SetShaderSource(ShaderSource::Ptr source)
    {
        if(source == nullptr)
        {
            return;
        }

        _source = source;
        ParseShaderIncludes(source->content.c_str());
    }

    void Shader::SetDefineMacros(const char *macros[], int length)
    {
        if(_source == nullptr)
        {
            return;
        }

        if(macros != nullptr)
        {
            InsertDefMacros(macros, length);
        }
    }

    void Shader::SetupCompile()
    {
        if(_source == nullptr)
        {
            return;
        }

        const char *source = _source->content.c_str();
        glShaderSource(_res.id, 1, &source, NULL);

        for(auto includeSource : _includeSouces)
        {
            if (includeSource != nullptr)
            {
                // std::regex filenameRegex("/([a-zA-Z0-9_-]+/{1})*[a-zA-Z0-9_-]+.glsl");
                // std::smatch sm;
                // std::string filename = includeSource->filename;
                // std::regex_search(filename, sm, filenameRegex);

                if (!includeSource->filename.empty() &&
                    !includeSource->content.empty())
                {
                    // std::string includeFilename = sm.str();

                    glNamedStringARB(GL_SHADER_INCLUDE_ARB
                        , includeSource->filename.size()
                        , includeSource->filename.c_str()
                        , includeSource->content.size()
                        , includeSource->content.c_str()
                    );
#ifdef _DEBUG
                    GLenum error = glGetError();
                    const GLubyte *error_str = glGetString(error);
#endif
                }
            }
        }
#ifdef _DEBUG
        GLenum error = glGetError();
        const GLubyte *error_str = glGetString(error);
#endif

        _setupCompile = true;
    }

    bool Shader::Compile()
    {
        if(!_setupCompile)
        {
            return false;
        }

        if(!_compiled)
        {
            for (auto includeSource : _includeSouces)
            {
                if (includeSource != nullptr)
                {
                    int32 position = includeSource->filename.find_last_of("/");
                    if(position != -1)
                    {
                        std::string path = includeSource->filename.substr(0, position);
                        const char *const cpath = path.c_str();
                        glCompileShaderIncludeARB(_res.id, 1, &cpath, nullptr);
                    }
                }
            }

            glCompileShader(_res.id);

            // release shadersouce
            _source = nullptr;
        }

#ifdef _DEBUG
        GLenum error = glGetError();
        const GLubyte *error_str = glGetString(error);
#endif

        //opengl programing guide 8th source code
        GLint compiled;
        glGetShaderiv(_res.id, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
#ifdef _DEBUG
            GLsizei len;
            glGetShaderiv(_res.id, GL_INFO_LOG_LENGTH, &len);

            GLchar *log = new GLchar[len + 1];
            glGetShaderInfoLog(_res.id, len, &len, log);
            Console::LogError("Shader: Shader ", _res.id, "  compilation failed: ", log, "\n");
            SAFE_DEL_ARR(log);
#endif /* DEBUG */

            glDeleteShader(_res.id);
        }

        _compiled = compiled;
        return _compiled;
    }

    bool Shader::CheckShadingLanguageIncludeARB()
    {
        return CheckSupportExtension("GL_ARB_shading_language_include");
    }

    void Shader::InsertDefMacros(const char *defMacros[], int length)
    {
        if(_source == nullptr)
        {
            return;
        }

        bool versionMatch = false;
        int32 position = -1;
        std::istringstream input(_source->content.c_str());
        for (std::array<char, 256> out; input.getline(&out[0], 256);)
        {
            std::regex extensionRegex("\\s*#extension\\s+");
            std::regex versionRegex("\\s*#version\\s*[0-9]*\\score\\s*");
            std::smatch sm;
            std::string extString(&out[0]);

            if(!versionMatch && std::regex_match(&out[0], versionRegex))
            {
                position = input.tellg();
                versionMatch = true;
                continue;
            }

            if(std::regex_search(extString, sm, extensionRegex))
            {
                position = input.tellg();
                continue;
            }
        }

        if(position != -1)
        {
            std::string preStr = _source->content.substr(0, position - 1) + "\n";
            std::string postStr = _source->content.substr(position, _source->content.length() - position);
            for(int32 i = 0; i < length; i++)
            {
                preStr = preStr + defMacros[i];
                // _source->content.insert(position - 1, )

            }
            _source->content = preStr + postStr;
        }
    }

    void Shader::ParseShaderIncludes(const char *source)
    {
        if(source == nullptr)
        {
            return;
        }

        std::istringstream input(source);
        for (std::array<char, 256> out; input.getline(&out[0], 256);)
        {
            std::regex includeRegex("\\s*#include\\s*</([a-zA-Z0-9_-]+/{1})*[a-zA-Z0-9_-]+.glsl>\\s*");
            bool match = std::regex_match(&out[0], includeRegex);
            if(match)
            {
                std::regex pathRegex("/([a-zA-Z0-9_-]+/{1})*[a-zA-Z0-9_-]+.glsl");
                std::smatch sm;
                std::string pathString(&out[0]);
                std::regex_search(pathString, sm, pathRegex);

                int32 position = _source->filename.find_last_of("/");
                if(position == -1)
                {
                    position = _source->filename.find_last_of("\\");
                }

                std::string directory = _source->filename.substr(0, position) + "/";
                TextManagerInst textMgr;
                TextReadInfo textReadInfo({ directory + sm.str() });
                ShaderIncludeSource::Ptr includeSource = textMgr->ReadText(textReadInfo);

                assert(includeSource != nullptr);
                includeSource->filename = sm.str();
                ParseShaderIncludes(includeSource->content.c_str());
                _includeSouces.push_back(includeSource);

                Console::LogInfo("ShaderInclude: Shader ", _res.id, " include ", sm.str(), "\n");
            }
        }
    }

    /**
 * @brief Construct a new Shader Program:: Shader Program object
 * 
 */
    ShaderProgram::ShaderProgram(Shader::Ptr* shaders, int32 num)
        : Object(), _res()
    {
        for(int32 i = 0; i < num; i++)
        {
            _shaders.push_back(shaders[i]);
        }

        _res.id = glCreateProgram();
        _res.type = GL_PROGRAM;

#ifdef _DEBUG
        GLenum error = glGetError();
        const GLubyte *error_str = glGetString(error);
#endif
    }

    ShaderProgram::~ShaderProgram()
    {
        Console::LogInfo("Shader: Shader Program ", _res.id, "(hash: ", _res.hash, ") deconstruct\n");

        _shaders.clear();
        glDeleteProgram(_res.id);
    }

    bool ShaderProgram::Link()
    {
        if(_shaders.size() == 0)
        {
            return false;
        }

        int32 num = _shaders.size();
        for (int i = 0; i < num; i++)
        {
            glAttachShader(_res.id, _shaders[i]->GetRes().id);
        }

        //link program
        glLinkProgram(_res.id);

        //opengl programing guide 8th source code
        glGetProgramiv(_res.id, GL_LINK_STATUS, &_linked);
        if (!_linked)
        {
#ifdef _DEBUG
            GLsizei len;
            glGetProgramiv(_res.id, GL_INFO_LOG_LENGTH, &len);

            GLchar *log = new GLchar[len + 1];
            glGetProgramInfoLog(_res.id, len, &len, log);
            Console::LogError("Shader: Shader linking failed: ", log, "\n");
            SAFE_DEL_ARR(log);
#endif /* DEBUG */

            for (int i = 0; i < num; i++)
            {
                glDetachShader(_res.id, _shaders[i]->GetRes().id);
            }

            glDeleteProgram(_res.id);
        }
        else
        {
            for (int i = 0; i < num; i++)
            {
                glDetachShader(_res.id, _shaders[i]->GetRes().id);
            }
        }

        // release shaders
        _shaders.clear();

        return _linked;
    }

    int32 ShaderProgram::GetActiveAtrtribsCount()
    {
        int32 count;
        glGetProgramiv(_res.id, GL_ACTIVE_ATTRIBUTES, &count);

        return count;
    }

    int32 ShaderProgram::GetActiveUniformBlocksCount()
    {
        int32 count;
        glGetProgramiv(_res.id, GL_ACTIVE_UNIFORM_BLOCKS, &count);

        return count;
    }

    int32 ShaderProgram::GetActiveUniformsCount()
    {
        int32 count;
        glGetProgramiv(_res.id, GL_ACTIVE_UNIFORMS, &count);

        return count;
    }

    void ShaderProgram::GetActiveAttribute(uint32 index, char *name)
    {
        int32 length;
        int32 size;
        GLenum type;
        glGetActiveAttrib(_res.id, index, 256, &length, &size, &type, name);
    }

    void ShaderProgram::GetActiveUniform(uint32 index, char *name)
    {
        int32 length;
        int32 size;
        GLenum type;
        glGetActiveUniform(_res.id, index, 256, &length, &size, &type, name);
    }

    int32 ShaderProgram::GetActiveUniformBlock(uint32 index)
    {
        int32 block;

        return block;
    }

    int32 ShaderProgram::GetAttributeLocation(char *name)
    {
        return glGetAttribLocation(_res.id, name);
    }

    /**
 * @brief Construct a new Shader Resource:: Shader Resource object
 * 
 */
    ShaderResource::ShaderResource()
        : Resource()
    {
    }

    ShaderResource::~ShaderResource()
    {
    }

    /**
 * @brief Construct a new Shader Reader:: Shader Reader object
 * 
 * @param read_info 
 */
    ShaderReader::ShaderReader(ShaderReadInfo &read_info)
        : _readInfo(read_info)
    {
    }

    ShaderReader::~ShaderReader()
    {
    }

    /**
     * @brief 
     * 
     * @tparam  
     * @param filename 
     * @param option 
     * @return ReadResult<Shader::Ptr> 
     */
    template <>
    ReadResult<Shader::Ptr> ShaderReader::Read<Shader::Ptr>(const char *filename, ReaderOption *option)
    {
        FILE *fp;
        fp = fopen(filename, "rb");
        if (fp)
        {
            Console::LogInfo("Shader: ShaderReader open shader file ", filename, " successed.\n");

            //opengl programing guide 8th source code
            //read source
            fseek(fp, 0, SEEK_END);
            int len = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            GLchar *source = new GLchar[len + 1];
            fread(source, 1, len, fp);
            fclose(fp);

            source[len] = 0;
            // Shader::Ptr sharedShader = std::make_shared<Shader>(_readInfo.type, source);

            ShaderSource::Ptr sourcePtr = std::make_shared<ShaderSource>();
            sourcePtr->filename = std::string(filename);
            sourcePtr->content = std::string(source);
            SAFE_DEL_ARR(source);

            Shader::Ptr sharedShader = std::make_shared<Shader>(_readInfo.type, sourcePtr);
            sharedShader->SetDefineMacros(const_cast<const char**>(_readInfo.macros), _readInfo.numMacros);
            sharedShader->SetupCompile();

            if(_readInfo.compileImmediate)
            {
                if (!(sharedShader->Compile()))
                {
                    return ReadResult<Shader::Ptr>(ReadResult<Shader::Ptr>::Status::FAILED);
                }
            }

            return ReadResult<Shader::Ptr>(sharedShader, ReadResult<Shader::Ptr>::Status::SUCCESS);
        }
        else
        {
#ifdef _DEBUG
            Console::LogError("Shader: ShaderReader open shader file ", filename, " failed\n");
#endif
            return ReadResult<Shader::Ptr>(ReadResult<Shader::Ptr>::Status::FAILED);
        }

        return ReadResult<Shader::Ptr>();
    }

} // namespace TwinkleGraphics