#include "ScriptEngine.hpp"
#include "../util/assert.hpp"
#include "../util/stringutils.hpp"
#include "../util/Exception.hpp"
#include "../scene/sq_binding/sq_scene.hpp"

//#include "../math/as_binding/as_math.hpp"
//#include "../system/console/as_binding/as_console.hpp"

namespace sn
{

//------------------------------------------------------------------------------
//void asCheck(int returnCode)
//{
//    SN_ASSERT(returnCode >= 0, "An internal AngelScript error occurred. Code: " << returnCode);
//}

//------------------------------------------------------------------------------
// Implement a simple message callback function
//void asMessageCallback(const asSMessageInfo *msg, void *param)
//{
//    std::stringstream ss;
//    ss << msg->section << ": (" << msg->row << ", " << msg->col << "): " << msg->message;
//    std::string s = ss.str();
//
//    switch (msg->type)
//    {
//    case asMSGTYPE_ERROR:
//        SN_ERROR(s);
//        break;
//
//    case asMSGTYPE_WARNING:
//        SN_WARNING(s);
//        break;
//
//    case asMSGTYPE_INFORMATION:
//        SN_LOG(s);
//        break;
//    }
//}

//------------------------------------------------------------------------------
//void asPrint(std::string & msg)
//{
//    sn::Log::get().print(SN_LTM_DEBUG, msg);
//}

//------------------------------------------------------------------------------
//void asPrintError(std::string & msg)
//{
//    sn::Log::get().print(SN_LTM_ERROR, msg);
//}

//==============================================================================
// ScriptEngine
//==============================================================================

//------------------------------------------------------------------------------
ScriptEngine::ScriptEngine(Application & app) :
    r_app(app),
    m_squirrelVM(nullptr)
{}

//------------------------------------------------------------------------------
ScriptEngine::~ScriptEngine()
{
    if (m_squirrelVM)
    {
        sq_close(m_squirrelVM);
    }
}

//------------------------------------------------------------------------------
void ScriptEngine::initialize()
{
    SN_ASSERT(m_squirrelVM == nullptr, "initialize called twice");

    SN_LOG("Initializing script engine");

    // Create the script engine
    m_squirrelVM = sq_open(1024);
    Sqrat::DefaultVM::Set(m_squirrelVM);

    registerCoreAPI();
}

//------------------------------------------------------------------------------
bool ScriptEngine::compileSquirrelModule(const std::string & modName, const std::string & modNamespace, const std::vector<String> & files)
{
    // Compile scripts
    std::vector<Sqrat::Script> scripts;
    for (auto it = files.begin(); it != files.end(); ++it)
    {
        const String & filePath = *it;
        try
        {
            Sqrat::Script script(m_squirrelVM);
            script.CompileFile(toString(filePath));
            scripts.push_back(script);
        }
        catch (Sqrat::Exception ex)
        {
            SN_ERROR(ex.Message());
            return false;
        }
    }

    // Run scripts
    for (auto it = scripts.begin(); it != scripts.end(); ++it)
    {
        auto & script = *it;
        script.Run();
    }

    return true;
}

//------------------------------------------------------------------------------
void ScriptEngine::registerCoreAPI()
{
    SN_LOG("Registering core API");

    // Register math functions
    // TODO

    // Register print functions
    // TODO

    // Time
    // TODO
    //register_Time(m_engine);
    //register_Clock(m_engine);

    // Console stuff
    // TODO
	//register_console(*m_engine);

    // Register Entity system
    registerEntity(m_squirrelVM);
    registerScene(m_squirrelVM);
}

} // namespace sn

