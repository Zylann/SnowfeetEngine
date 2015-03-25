#include "PLYLoader.hpp"

namespace sn  {
namespace render {

// Some references
// http://paulbourke.net/dataformats/ply/

// I wrote this code very fast by retro-engineering and quick reading.
// Once it works, this needs improvements.

// TODO Tokenize first, then parse
// TODO Handle more cases
// TODO Factorise and optimize

namespace ply
{
    // Supported tags

    const char * firstLine = "ply";
    const char * comment = "comment";
    const char * format = "format";
    const char * property = "property";
    const char * element = "element";
    const char * end_header = "end_header";

    // Helpers

    inline f32 readColorField(std::ifstream & ifs, PLYDataType type)
    {
        f32 x;
        ifs >> x;

        switch (type)
        {
        case SNR_PLY_FLOAT:
        case SNR_PLY_DOUBLE:
            return x;

        default:
            return x / 255.f;
        }
    }

    inline void nextLine(std::ifstream & ifs)
    {
        while (!ifs.eof())
        {
            char c = ifs.get();
            if (c == '\n' || c == '\r')
                break;
        }
        while (!ifs.eof())
        {
            char c = ifs.peek();
            if (c == '\n' || c == '\r')
                ifs.get();
            else
                break;
        }
    }
}

PLYLoader::PLYLoader(std::ifstream & ifs):
    m_isHeader(true),
    r_ifs(ifs),
    r_mesh(nullptr)
{
    m_elementTypeToID["vertex"] = SNR_PLY_VERTEX;
    m_elementTypeToID["face"] = SNR_PLY_FACE;

    m_dataTypeToID["char"] = SNR_PLY_CHAR;
    m_dataTypeToID["short"] = SNR_PLY_CHAR;
    m_dataTypeToID["int"] = SNR_PLY_CHAR;
    m_dataTypeToID["uchar"] = SNR_PLY_UCHAR;
    m_dataTypeToID["ushort"] = SNR_PLY_USHORT;
    m_dataTypeToID["uint"] = SNR_PLY_UINT;
    m_dataTypeToID["float"] = SNR_PLY_FLOAT;
    m_dataTypeToID["double"] = SNR_PLY_DOUBLE;

    m_fieldToID["x"] = SNR_PLY_X;
    m_fieldToID["y"] = SNR_PLY_Y;
    m_fieldToID["z"] = SNR_PLY_Z;
    m_fieldToID["nx"] = SNR_PLY_NX;
    m_fieldToID["ny"] = SNR_PLY_NY;
    m_fieldToID["nz"] = SNR_PLY_NZ;
    m_fieldToID["s"] = SNR_PLY_S;
    m_fieldToID["t"] = SNR_PLY_T;
    m_fieldToID["red"] = SNR_PLY_RED;
    m_fieldToID["green"] = SNR_PLY_GREEN;
    m_fieldToID["blue"] = SNR_PLY_BLUE;
    m_fieldToID["vertex_indices"] = SNR_PLY_VERTEX_INDICES;
}

bool PLYLoader::loadMesh(Mesh & out_mesh)
{
    r_mesh = &out_mesh;

    // Check first line
    std::string line;
    r_ifs >> line;
    if (line != ply::firstLine)
    {
        SN_ERROR("PLY file not beginning with " << ply::firstLine);
        return false;
    }

    // Parse header
    m_isHeader = true;
    if (!parseHeader())
        return false;

    out_mesh.clear();

    return parseTriangles();
}

bool PLYLoader::parseHeader()
{
    std::ifstream & ifs = r_ifs;

    // Parse header
    while (!ifs.eof() && m_isHeader)
    {
        std::string command;
        ifs >> command;

        if (command == ply::format)
        {
            std::string format;
            std::string version;

            ifs >> format;
            ifs >> version;

            m_format = format + ' ' + version;

            if (m_format != "ascii 1.0")
            {
                SN_ERROR("PLY format not supported: " << m_format);
                return false;
            }
        }
        else if (command == ply::element)
        {
            std::string elementTypeStr;
            ifs >> elementTypeStr;
            auto it = m_elementTypeToID.find(elementTypeStr);
            if (it == m_elementTypeToID.end())
            {
                SN_ERROR("PLY element type not supported: " << elementTypeStr);
                return false;
            }

            PLYElement element;
            element.type = it->second;
            ifs >> element.count;
            m_elements.push_back(element);
        }
        else if (command == ply::property)
        {
            PLYElement & currentElement = m_elements.back();
            PLYProperty property;

            if (m_elements.empty())
            {
                SN_ERROR("PLY property specified outside element");
                return false;
            }

            std::string typeStr;
            ifs >> typeStr;
            if (typeStr == "list")
            {
                ifs >> typeStr;
                auto dataTypeIt2 = m_dataTypeToID.find(typeStr);
                if (dataTypeIt2 == m_dataTypeToID.end())
                {
                    SN_ERROR("PLY data type not supported: " << typeStr);
                    return false;
                }

                ifs >> typeStr;
                auto dataTypeIt3 = m_dataTypeToID.find(typeStr);
                if (dataTypeIt3 == m_dataTypeToID.end())
                {
                    SN_ERROR("PLY data type not supported: " << typeStr);
                    return false;
                }

                property.isList = true;
                property.listCountType = dataTypeIt2->second;
                property.type = dataTypeIt3->second;
            }
            else // Scalar
            {
                auto dataTypeIt = m_dataTypeToID.find(typeStr);
                if (dataTypeIt == m_dataTypeToID.end())
                {
                    SN_ERROR("PLY data type not supported: " << typeStr);
                    return false;
                }
                property.type = dataTypeIt->second;
            }

            std::string fieldStr;
            ifs >> fieldStr;
            auto fieldIt = m_fieldToID.find(fieldStr);
            if (fieldIt == m_fieldToID.end())
            {
                SN_ERROR("PLY field not supported: " << fieldStr);
                return false;
            }
            property.field = fieldIt->second;

            currentElement.ownedFields.insert(property.field);
            currentElement.properties.push_back(property);
        }
        else if (command == ply::end_header)
        {
            m_isHeader = false;
        }
        else if (command == ply::comment)
        {
            // Ignore this line
            ply::nextLine(ifs);
            continue;
        }
        else
        {
            SN_ERROR("PLY unknown/unsupported tag '" << command << "'");
            return false;
        }
    }

    if (ifs.eof() && m_isHeader)
    {
        SN_ERROR("PLY unexpected end of file while reading header");
        return false;
    }

    return true;
}

bool PLYLoader::parseTriangles()
{
    Mesh & out_mesh = *r_mesh;

    for (auto it = m_elements.begin(); it != m_elements.end(); ++it)
    {        
        PLYElement & element = *it;
        switch (element.type)
        {
        case SNR_PLY_VERTEX:
            if (!parseVertices(element))
                return false;
            break;

        case SNR_PLY_FACE:
            if (!parseFaces(element))
                return false;
            break;

        default:
            SN_ASSERT(false, "PLY invalid state");
            return false;
        }
    }

    return true;
}

bool PLYLoader::parseVertices(const PLYElement & element)
{
    std::ifstream & ifs = r_ifs;

    std::vector<Vector3f> positions;
    std::vector<Vector3f> normals;
    std::vector<Vector2f> texCoords;
    std::vector<Color> colors;

    for (u32 j = 0; j < element.count; ++j)
    {
        if (element.ownedFields.find(SNR_PLY_X) != element.ownedFields.end()) positions.push_back(Vector3f());
        if (element.ownedFields.find(SNR_PLY_NX) != element.ownedFields.end()) normals.push_back(Vector3f());
        if (element.ownedFields.find(SNR_PLY_S) != element.ownedFields.end()) texCoords.push_back(Vector2f());
        if (element.ownedFields.find(SNR_PLY_RED) != element.ownedFields.end()) colors.push_back(Color());

        for (u32 i = 0; i < element.properties.size(); ++i)
        {
            const PLYProperty & property = element.properties[i];
            switch (property.field)
            {
            case SNR_PLY_X: ifs >> positions.back().x(); break;
            case SNR_PLY_Y: ifs >> positions.back().y(); break;
            case SNR_PLY_Z: ifs >> positions.back().z(); break;

            case SNR_PLY_NX: ifs >> normals.back().x(); break;
            case SNR_PLY_NY: ifs >> normals.back().y(); break;
            case SNR_PLY_NZ: ifs >> normals.back().z(); break;

            case SNR_PLY_S: ifs >> texCoords.back().x(); break;
            case SNR_PLY_T: ifs >> texCoords.back().y(); break;

            case SNR_PLY_RED:    colors.back().r = ply::readColorField(ifs, property.type); break;
            case SNR_PLY_GREEN:  colors.back().g = ply::readColorField(ifs, property.type); break;
            case SNR_PLY_BLUE:   colors.back().b = ply::readColorField(ifs, property.type); break;

            default: break;
            }
        }
    }

    // For test purpose
    //for (auto it = colors.begin(); it != colors.end(); ++it)
    //{
    //    *it = Color(math::randf(), math::randf(), math::randf());
    //}

    Mesh & out_mesh = *r_mesh;

    if (!positions.empty()) 
        out_mesh.setPositions(&positions[0], positions.size());

    if (!normals.empty()) 
        out_mesh.setNormals(&normals[0], normals.size());

    if (!colors.empty()) 
        out_mesh.setColors(&colors[0], colors.size());

    if (!texCoords.empty()) 
        out_mesh.setUV(&texCoords[0], texCoords.size());

    return true;
}

bool PLYLoader::parseFaces(const PLYElement & element)
{
    std::ifstream & ifs = r_ifs;

    std::vector<u32> indices;

    if (element.properties.size() != 1)
    {
        SN_ERROR("PLY Faces must have only one property");
        return false;
    }

    for (u32 j = 0; j < element.count; ++j)
    {
        for (u32 i = 0; i < element.properties.size(); ++i)
        {
            const PLYProperty & property = element.properties[i];
            if (property.isList)
            {
                u32 count = 0;
                ifs >> count;
                if (count == 4)
                {
                    u32 quadIndices[4];
                    for (u32 n = 0; n < count; ++n)
                    {
                        ifs >> quadIndices[n];
                    }
                    // Make two triangles
                    indices.push_back(quadIndices[2]);
                    indices.push_back(quadIndices[3]);
                    indices.push_back(quadIndices[1]);
                    indices.push_back(quadIndices[3]);
                    indices.push_back(quadIndices[0]);
                    indices.push_back(quadIndices[1]);
                }
                else if (count == 3)
                {
                    for (u32 n = 0; n < count; ++n)
                    {
                        u32 index = 0;
                        ifs >> index;
                        indices.push_back(index);
                    }
                }
                else
                {
                    SN_ERROR("PLY Faces indices are not quads or triangles (count: " << count << ")");
                    return false;
                }
            }
        }
    }

    r_mesh->setTriangleIndices(&indices[0], indices.size());

    return true;
}

} // namespace render
} // namespace sn


