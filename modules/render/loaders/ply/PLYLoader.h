#ifndef __HEADER_SNR_PLY_LOADER__
#define __HEADER_SNR_PLY_LOADER__

#include <fstream>
#include <unordered_set>
#include <unordered_map>

#include <modules/render/Mesh.h>

// Disclaimer: this loader is only intented for very basic files, and sometimes might not work.
// As SDL can load BMP images without SDL_Image, this mod can load one simple type
// of model format without third-party library.
// For more complex models, use the appropriate module.

namespace sn
{

enum PLYElementType
{
    SNR_PLY_VERTEX = 0,
    SNR_PLY_FACE
};

enum PLYField
{
    SNR_PLY_X = 0,
    SNR_PLY_Y,
    SNR_PLY_Z,
    SNR_PLY_NX,
    SNR_PLY_NY,
    SNR_PLY_NZ,
    SNR_PLY_S,
    SNR_PLY_T,
    SNR_PLY_RED,
    SNR_PLY_GREEN,
    SNR_PLY_BLUE,
    SNR_PLY_VERTEX_INDICES
};

enum PLYDataType
{
    SNR_PLY_CHAR = 0,
    SNR_PLY_SHORT,
    SNR_PLY_INT,
    SNR_PLY_UCHAR,
    SNR_PLY_USHORT,
    SNR_PLY_UINT,
    SNR_PLY_FLOAT,
    SNR_PLY_DOUBLE
};

struct PLYProperty
{
    PLYProperty(): isList(false) {}

    PLYDataType type;
    PLYField field;
    bool isList;
    PLYDataType listCountType;
};

struct PLYElement
{
    PLYElement() : count(0) {}

    size_t count;
    PLYElementType type;
    std::vector<PLYProperty> properties;
    std::unordered_set<PLYField> ownedFields;
};

/// \brief Reads a PLY stream and converts its data into a mesh supported by the rendering engine.
/// \note This loader is suited for simple 3D models (no animations, sub-objects or embedded materials).
class PLYLoader
{
public:
    PLYLoader(std::ifstream & ifs);

    bool loadMesh(Mesh & out_mesh);

private:
    bool parseHeader();
    bool parseTriangles();
    bool parseVertices(const PLYElement & element);
    bool parseFaces(const PLYElement & element);

private:
    // Parser state
    bool m_isHeader;
    std::ifstream & r_ifs;
    Mesh * r_mesh; // Target mesh

    // Metadata
    std::string m_format;

    // Data description
    std::vector<PLYElement> m_elements;

    // Tags mapping
    std::unordered_map<std::string, PLYElementType> m_elementTypeToID;
    std::unordered_map<std::string, PLYDataType> m_dataTypeToID;
    std::unordered_map<std::string, PLYField> m_fieldToID;

};

} // namespace sn

#endif // __HEADER_SNR_PLY_LOADER__

