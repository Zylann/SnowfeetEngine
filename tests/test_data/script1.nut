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

function test() {
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

//------------------------------------------------------------------------------
test()

