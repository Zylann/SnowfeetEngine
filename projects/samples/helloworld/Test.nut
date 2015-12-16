class HelloTest
{
	entity = null
	_myName = "The bum"
	_updateCount = 0

	constructor() {
		print("hello.Test has been created!")
	}

	function onUnserialize() {
		print("onUnserialize was called! I am " + _myName)
		onUpdate()
	}

	function onReady() {
		print("onReady was called!");
	}

	function onUpdate() {
		print("Update number " + (++_updateCount))
	}
}

print("Hello has been compiled!")
