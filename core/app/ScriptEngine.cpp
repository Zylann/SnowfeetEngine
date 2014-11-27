#include "../angelscript/addons/scriptstdstring/scriptstdstring.h"
#include "../angelscript/addons/scriptarray/scriptarray.h"
#include "../angelscript/addons/scriptmath/scriptmath.h"

#include "../angelscript/addons/scriptbuilder/scriptbuilder.h"

#include "ScriptEngine.hpp"
#include "../util/assert.hpp"
#include "../util/stringutils.hpp"
#include "../system/time/as_time.hpp"
#include "../util/Exception.hpp"

#include "../math/as_binding/as_math.hpp"

#include "../system/console/as_binding/as_console.hpp"

namespace sn
{

//------------------------------------------------------------------------------
void asCheck(int returnCode)
{
    SN_ASSERT(returnCode >= 0, "An internal AngelScript error occurred. Code: " << returnCode);
}

//------------------------------------------------------------------------------
// Implement a simple message callback function
void asMessageCallback(const asSMessageInfo *msg, void *param)
{
    std::stringstream ss;
    ss << msg->section << ": (" << msg->row << ", " << msg->col << "): " << msg->message;
    std::string s = ss.str();

    switch (msg->type)
    {
    case asMSGTYPE_ERROR:
        SN_ERROR(s);
        break;

    case asMSGTYPE_WARNING:
        SN_WARNING(s);
        break;

    case asMSGTYPE_INFORMATION:
        SN_LOG(s);
        break;
    }
}

//------------------------------------------------------------------------------
void asPrint(std::string & msg)
{
    sn::Log::get().print(SN_LTM_DEBUG, msg);
}

//------------------------------------------------------------------------------
void asPrintError(std::string & msg)
{
    sn::Log::get().print(SN_LTM_ERROR, msg);
}

//==============================================================================
// ScriptEngine
//==============================================================================

//------------------------------------------------------------------------------
ScriptEngine::ScriptEngine(Application & app) :
    r_app(app),
    m_engine(nullptr),
    m_context(nullptr)
{}

//------------------------------------------------------------------------------
ScriptEngine::~ScriptEngine()
{
    if (m_context)
    {
        m_context->Release();
        m_context = nullptr;
    }
    if (m_engine)
    {
        m_engine->Release();
        m_engine = nullptr;
    }
}

//------------------------------------------------------------------------------
void ScriptEngine::initialize()
{
    SN_LOG("Initializing script engine");
    SN_LOG("AngelScript version: " << ANGELSCRIPT_VERSION_STRING);

    // Create the script engine
    m_engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

    // Set the message callback to receive information on errors in human readable form.
    int r = m_engine->SetMessageCallback(asFUNCTION(asMessageCallback), 0, asCALL_CDECL);
    SN_ASSERT(r >= 0, "AngelScript: message callback setting error");

    registerCoreAPI();

}

//------------------------------------------------------------------------------
bool ScriptEngine::compileModule(std::string modName, std::string modNamespace, const std::vector<String> & files)
{
    // The CScriptBuilder helper is an add-on that loads the file,
    // performs a pre-processing pass if necessary, and then tells
    // the engine to build a script module.
    CScriptBuilder builder;
    int r = builder.StartNewModule(m_engine, modName.c_str());
    if (r < 0)
    {
        // If the code fails here it is usually because there
        // is no more memory to allocate the module
        SN_ERROR("Unrecoverable error while starting compilation of module \"" << modName << '"');
        return false;
    }

    for (u32 i = 0; i < files.size(); ++i)
    {
        r = builder.AddSectionFromFile(toString(files[i]).c_str());
        if (r < 0)
        {
            // The builder wasn't able to load the file. Maybe the file
            // has been removed, or the wrong name was given, or some
            // preprocessing commands are incorrectly written.
            SN_ERROR("Please correct the errors in the script and try again.");
            return false;
        }
    }

    r = builder.BuildModule();
    if (r < 0)
    {
        // An error occurred. Instruct the script writer to fix the
        // compilation errors that were listed in the output stream.
        SN_ERROR("Please correct the errors in the script and try again.");
        return false;
    }

    // Call compile-time callback function if present

    std::string compiledFuncDecl;
    if (modNamespace.empty())
        compiledFuncDecl = "void __compiled__()";
    else
        compiledFuncDecl = "void " + modNamespace + "::__compiled__()";

    // TODO iterate over namespaces

    asIScriptModule *mod = m_engine->GetModule(modName.c_str());
    asIScriptFunction *func = mod->GetFunctionByDecl(compiledFuncDecl.c_str());
    if (func != 0)
    {
        // Create our context, prepare it, and then execute
        asIScriptContext *ctx = getContext();
        ctx->Prepare(func);
        executeContext(*ctx);
    }
    else
    {
        SN_DLOG("No " << compiledFuncDecl << " function found");
    }

    return true;
}

//------------------------------------------------------------------------------
void ScriptEngine::registerCoreAPI()
{
    SN_LOG("Registering core API");

    //
    // Global API
    //

    // Register string:
    // AngelScript doesn't have a built-in string type, as there is no definite standard
    // string type for C++ applications. Every developer is free to register it's own string type.
    // The SDK do however provide a standard add-on for registering a string type, so it's not
    // necessary to implement the registration yourself if you don't want to.
    RegisterStdString(m_engine);

    // Register AS math functions
    RegisterScriptMath(m_engine);

    // Register array type
    RegisterScriptArray(m_engine, true);

    // Register print functions
    asCheck(m_engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(asPrint), asCALL_CDECL));
    asCheck(m_engine->RegisterGlobalFunction("void error(const string &in)", asFUNCTION(asPrintError), asCALL_CDECL));

    // Time
    register_Time(m_engine);
    register_Clock(m_engine);

    // Core math
    register_math(*m_engine);

    //
    // Namespaced API
    //

	register_console(*m_engine);
}

//------------------------------------------------------------------------------
asIScriptEngine * ScriptEngine::getEngine()
{
    return m_engine;
}

//------------------------------------------------------------------------------
asIScriptContext * ScriptEngine::getContext()
{
    if (m_engine && m_context == nullptr)
    {
        m_context = m_engine->CreateContext();
    }
    return m_context;
}

//------------------------------------------------------------------------------
bool ScriptEngine::executeContext(asIScriptContext & context)
{
    s32 r = context.Execute();
    if (r != asEXECUTION_FINISHED)
    {
        // The execution didn't complete as expected. Determine what happened.
        if (r == asEXECUTION_EXCEPTION)
        {
            // An exception occurred, let the script writer know what happened so it can be corrected.
            const char * sectionName = 0;
            s32 column = 0;
            s32 line = context.GetExceptionLineNumber(&column, &sectionName);
            SN_ERROR(sectionName << ": (" << line << ", " << column << "): "
                "An exception " << context.GetExceptionString() << " occurred.");
        }
        else
        {
            SN_ERROR("An error occurred while executing a script function (code: " << r << ").");
        }
        return false;
    }
    else
    {
        return true;
    }
}


} // namespace sn

