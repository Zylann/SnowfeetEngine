#ifndef __HEADER_SN_TAGMANAGER_HPP__
#define __HEADER_SN_TAGMANAGER_HPP__

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <core/types.hpp>
#include <JsonBox.h>

namespace sn
{

class Entity;

/// \brief associates names and numbers as tags to quickly identify entities
class SN_API TagManager
{
public:

    /// \brief Maximum number of tags that can be defined
    static const u32 MAX_TAGS = 32;

    /// \brief Gives a name to a tag, making it defined in some way.
    /// \note You can have unnamed tags, but they can't be used from their names.
    /// It's strongly recommended to name your tags.
    /// \return object itself for chaining
    TagManager & setTag(u8 index, const std::string & name);

    /// \brief Gets the name of a tag from its index.
    /// Undefined behaviour may happen if the index is out of bounds.
    /// \param index: index of the tag in [0,31]
    /// \return name of the tag, empty string if the tag has no defined name.
    const std::string & nameFromIndex(u8 index) const;

    /// \brief Gets the index of a tag from its name.
    /// \param name: name of the tag.
    /// \return index of the tag, -1 if not found.
    s8 indexFromName(const std::string & name) const;

    /// \brief Gets a collection of entities having the given tag.
    /// \param tagIndex: index of the tag to search.
    /// \return entities having the tag.
    const std::unordered_set<Entity*> & taggedEntities(u8 tagIndex) const;

    /// \brief Called when an entity gets tagged.
    /// Don't call this method yourself.
    void onEntityTagged(Entity * e, u8 tagIndex);

    /// \brief Called when an entity loses a tag or gets destroyed.
    /// Don't call this method yourself.
    void onEntityUntagged(Entity * e, u8 tagIndex);

    /// \brief Prints a human-readable text describing active tags.
    /// \param os: stream where to print text, usually std::cout.
    void print(std::ostream & os) const;

    /// \brief Saves the list of tags (not tagged entities).
    /// \param o: output JSON data
    void serialize(JsonBox::Value & o);

    /// \brief Loads the list of tags (not tagged entities).
    /// \param o: input JSON data
    void unserialize(JsonBox::Value & o);

private:

    std::unordered_map<std::string,u8> m_name2Tag;
    std::string m_tags[MAX_TAGS];
    std::unordered_set<Entity*> m_taggedEntities[MAX_TAGS];

};

} // namespace sn

#endif // __HEADER_SN_TAGMANAGER__


