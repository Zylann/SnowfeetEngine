#ifndef __HEADER_SN_OBJECTDB__
#define __HEADER_SN_OBJECTDB__

#include <iostream>
#include <vector>
#include <unordered_map>
#include <core/asset/Asset.h>

namespace sn
{

/// \brief Represents the serialized state of one or a collection of objects.
/// Parts of the data can have references inside itself,
/// or instantiate foreign assets of the same type with possible modifications on them (like prefabs).
/// This can be done recursively.
class ObjectDB : public Asset
{
public:
    SN_SCRIPT_OBJECT(sn::ObjectDB, sn::Asset)

    //---------------------------------------------
    // Constants
    //---------------------------------------------

    /// \brief Tag under which the format value is stored
	static const char * FORMAT_KEY;
    /// \brief Value identifying the format under which data is serialized.
	static const char * FORMAT_VALUE;

	static const char * SRC_TAG;
	static const char * REF_TAG;
	static const char * CHANGES_TAG;
	static const char * OBJECTS_TAG;
    static const char * NEXT_ID_TAG;
    static const char * ROOT_ID_TAG;

    static const u32 NO_ROOT = -1;

    //---------------------------------------------
    // Constructor
    //---------------------------------------------

    ObjectDB();

    //---------------------------------------------
    // Methods
    //---------------------------------------------

    /// \brief Loads the database from a stream (Expects JSON)
	virtual bool loadFromStream(std::istream & is);
    /// \brief Loads the database from a JSON document
    virtual bool loadFromVariant(Variant & doc);

    /// \brief Gets one object's data by its ID in the database
	const Variant * getObject(u32 id) const;
    /// \brief Gets the root object in this database, if defined. Can be null.
    const Variant * getRootObject() const;

    //---------------------------------------------
    // Public static helpers
    //---------------------------------------------

    /// \brief Gets the object ID represented by the given variant.
    /// \param o: variant representing the reference
    /// \param out_ref: retrieved object ID. Will not be modified if it is not found.
    /// \return true if the variant represented a reference, false otherwise
    static bool getRef(const Variant & o, u32 & out_ref);

protected:
    struct Object
    {
        // Object state
        Variant data;
    };

    /// \brief Called when the PackedObject needs to recalculate overriding results.
    /// Depending on the type of PackedObject, another asset type might be looked up.
    virtual ObjectDB * getFromAssetDatabase(const std::string & location) const;

    const std::unordered_map<u32, ObjectDB::Object> & getObjects() const { return m_objects; }

    bool isFlattened();
	void flatten();

private:
    struct Modification;
    struct OverrideObject;

	static bool validate(Variant & a_doc, u32 * out_nextID=nullptr);
    static void parseModifications(const Variant & doc, std::vector<Modification> & out_modifications);
    static void parsePath(const Variant & o, std::vector<std::string> & out_path);

	void flatten(std::vector<ObjectDB*> & stack);
    bool flattenObject(OverrideObject & overrideObj, u32 id, std::vector<ObjectDB*> & stack);

    static void applyChange(Variant & obj, Modification change);
    static void mapReferences(Variant & obj, const std::unordered_map<u32, u32> & refMap);

    void clear();
    void clearInstances();

    u32 makeID();

    Variant * getObject(u32 id);

private:
    //---------------------------------------------
    // Structures
    //---------------------------------------------

    struct Modification
    {
        // Target object to override (ID scope is the source object)
        u32 target;
        // Path to the member
        std::vector<std::string> path;
        // New value of the member
        Variant value;
    };

    struct OverrideObject
    {
        // Location of the inherited file, if any
        std::string src;
        // Object modifications from the original
        std::vector<Modification> overrides;
    };

private:
    //---------------------------------------------
    // Fields
    //---------------------------------------------

    /// \brief Next ID to be generated when an object is added
    u32 m_nextID;
    /// \brief ID of the main object
    u32 m_rootID;

    /// \brief Object instances from other files
    std::unordered_map<u32, OverrideObject> m_overridedObjects;
    /// \brief Ready-to-use flattened object data
    std::unordered_map<u32, Object> m_objects;

    /// \brief Tells if the file has been flattened (so instances are available as ready objects)
    bool m_isFlattened;
};

} // namespace sn

#endif // __HEADER_SN_OBJECTDB__

