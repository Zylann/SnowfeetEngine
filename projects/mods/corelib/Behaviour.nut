/// \brief Base class for an entity's scripted behaviour
class Behaviour
{
	/// \brief Entity this behaviour is currently attached to
	entity = null

	/// \brief Gets the scene the entity is currently in
	function getScene() {
		return entity.getScene()
	}
}

