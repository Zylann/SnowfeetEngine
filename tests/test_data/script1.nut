print("Hello from Squirrel\n")

//------------------------------------------------------------------------------
/// \brief Prints text in the log with a terminating newline.
/// \param msg: object to print. Must be convertible to a string.
function println(msg) {
	print(msg + "\n")
}

function printVar(v, title="(var)") {
	print("--- " + title + " ---\n")
	foreach(key,val in v) {
		print("- " + key + ": " + val + "\n")
		// TODO go recursive
	}
	print("\n")
}

function test1() {
	// Let's print the class object
	printVar(Something, "class Something")

	local c = Something()
	println("After creating c")
	
	// Call the function with intentionally more arguments than needed.
	// Shouldn't raise an error.
	c.doStuff(1, 2, 3)

	// Try some pointer thing
	local f = c.doStuff.bindenv(c)
	// Whoa!
	f()

	println("Before new test")
	c.setText("Bread")
	c.doStuff()
	println(c.getText())
}

function test2() {
	println("Beginning test 2")
	local child = test2_sub()
	child.sayHello()
}

function test2_sub() {
	local c = SharedThing()
	c.sayHello()
	local child = c.getChild()
	return child
}

function test3() {
	println("Beginning test 3")
	local c = DerivedThing()
	c.sayHello()
}

//------------------------------------------------------------------------------
test1()
test2()
test3()
