class ButtonScript
{
	entity = null
	parent = null

	constructor() {
		println("Hello from Squirrel!")
	}

	function onCreate() {
		println("onCreate was called")
		println("entity: " + entity)
	}

	function onButtonPress() {
		println("Wahaha someone pressed me")
		parent = entity.getParent()
	}
}
