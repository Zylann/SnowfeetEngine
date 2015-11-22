//------------------------------------------------------------------------------
/// \brief Prints text in the log with a terminating newline.
/// \param msg: object to print. Must be convertible to a string.
function println(msg) {
	print(msg + "\n")
}

//------------------------------------------------------------------------------
function printVar(v, title="var") {
	print("--- " + title + ":" + v + " ---\n")
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

//------------------------------------------------------------------------------
/// \brief Tries to get a value from a table, class or instance.
/// \param obj (class|instance|table)
/// \param key (var)
/// \param defval (optional var): returned value if the key is not found
/// \return obj[key] if it exists, defval otherwise
function tryget(obj, key, defval=null) {
	if(key in obj)
		return obj[key]
	return defval
}
