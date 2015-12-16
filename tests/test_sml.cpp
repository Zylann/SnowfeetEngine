#include "tests.hpp"

#include <core/sml/SmlParser.h>
#include <core/sml/SmlWriter.h>

using namespace sn;

namespace
{
    bool readValue(const std::string & fileName, SmlParser & parser, Variant & doc)
    {
        std::ifstream ifs(fileName.c_str(), std::ios::in | std::ios::binary);
        if (!ifs.good())
        {
            SN_ERROR("Couldn't open file " << fileName);
            return false;
        }
        parser.parseValue(ifs, doc);
        return true;
    }

    bool writeValue(const std::string & fileName, SmlWriter & writer, const Variant & doc)
    {
        std::ofstream ofs(fileName.c_str(), std::ios::out | std::ios::binary);
        if (!ofs.good())
        {
            SN_ERROR("Couldn't write file " << fileName);
            return false;
        }
        writer.writeValue(ofs, doc);
        return true;
    }
}

void test_sml()
{
    const std::string inputFileName = "test_data/scene.sml";
    const std::string outputPrettyFileName = "test_data/scene_output_pretty.sml";
    const std::string outputPrettyFileName2 = "test_data/scene_output_pretty2.sml";
    const std::string inputJsonFileName = "test_data/scene.json";
    const std::string outputPrettyFileName3 = "test_data/scene_output_pretty3.sml";

    SmlParser parser;
    SmlWriter writer(true);

    Variant doc;

    // Parse file
    SN_LOG("Reading SML file...");
    if (!readValue(inputFileName, parser, doc))
        return;

    // Write it to a file
    SN_LOG("Writing file...");
    if (!writeValue(outputPrettyFileName, writer, doc))
        return;

    // Read it back
    SN_LOG("Reading file back...");
    Variant doc2;
    if (!readValue(outputPrettyFileName, parser, doc2))
        return;

    // Write again to see if it matches
    SN_LOG("Writing file again...");
    writeValue(outputPrettyFileName2, writer, doc2);

    // Read JSON file
    SN_LOG("Reading JSON file...");
    Variant doc3;
    if (!readValue(inputJsonFileName, parser, doc3))
        return;

    // Write result
    SN_LOG("Writing JSON parsing result in file...");
    writeValue(outputPrettyFileName3, writer, doc3);
}

