//------------------------------------------------------------------------------
/// \brief Prints text in the log with a terminating newline.
/// \param msg: object to print. Must be convertible to a string.
function println(msg) {
	print(msg + "\n")
}

//------------------------------------------------------------------------------
function printVar(v, title="var") {
	print("--- " + title + ":" + typeof v + " ---\n")
	local vt = typeof v
	if(vt == "table" || vt == "class" || vt == "instance") {
		foreach(key,val in v) {
			print("- " + key + ": " + val)
			// TODO go recursive
		}
	}
	else {
		print(v == null)
	}
	print("\n")
}
