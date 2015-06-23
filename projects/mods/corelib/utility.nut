//------------------------------------------------------------------------------
/// \brief Prints text in the log with a terminating newline.
/// \param msg: object to print. Must be convertible to a string.
function println(msg) {
	print(msg + "\n")
}

//------------------------------------------------------------------------------
function printVar(v, title="(var)") {
	print("--- " + title + " ---\n")
	foreach(key,val in v) {
		print("- " + key + ": " + val + "\n")
		// TODO go recursive
	}
	print("\n")
}

