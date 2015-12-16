#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, json


header_extension = ".h"


def get_mod_lib_name(name):
	return "Mod" + name.title()


def create_cpp_folder(root, namespace, name):
	cppDir = root
	eol = "\n"
	indent = "    "

	headerName = "Module" + header_extension
	headerPath = os.path.join(cppDir, headerName)
	implemPath = os.path.join(cppDir, "Module" + ".cpp")
	premakePath = os.path.join(cppDir, "premake5.lua")

	libName = get_mod_lib_name(name)
	loadFunc = "int loadSnowfeetModule_" + libName + "(ModuleLoadArgs args)"
	unloadFunc = "int unloadSnowfeetModule_" + libName + "(ModuleUnloadArgs args)"
	regFuncName = "registerObjectTypes"
	regFunc = "void " + regFuncName + "(sn::ObjectTypeDatabase & otb)"

	exportMacro = "SN_MOD_EXPORT"

	guard = "__HEADER_MOD_" + namespace.upper() + "_" + name.upper() + "__"

	f = open(headerPath, "w+")
	f.write(eol.join([
		"#ifndef " + guard,
		"#define " + guard,
		"",
		"#include <core/app/ModuleArgs.hpp>",
		"",
		"extern \"C\"",
		"{",
		indent + exportMacro + " " + loadFunc + ";",
		indent + exportMacro + " " + unloadFunc + ";",
		"}",
		"",
		"#endif // " + guard,
		"",
		""
	]))
	f.close()

	f = open(implemPath, "w+")
	f.write(eol.join([
		"#include \"" + headerName + "\"",
		"",
		"",
		"namespace " + namespace,
		"{",
		indent + regFunc,
		indent + "{",
		indent + indent + "//otb.registerType<MyClass>();",
		indent + indent + "// TODO Register types",
		indent + "}",
		"}",
		"",
		loadFunc,
		"{",
		indent + namespace + "::" + regFuncName + "(*(args.objectTypeDatabase));",
		indent + "// TODO implement",
		indent + "return 0;",
		"}",
		"",
		unloadFunc,
		"{",
		indent + "// TODO implement",
		indent + "return 0;",
		"}",
		"",
		""
	]))
	f.close()

	f = open(premakePath, "w+")
	f.write(eol.join([
		"project \"" + libName + "\"",
		indent + "commonModConfigCPP()",
		indent + "files {",
		indent + indent + "\"**" + header_extension + "\",",
		indent + indent + "\"**.cpp\"",
		indent + "}",
		# indent + "filter \"configurations:Debug\"",
		# indent + indent + "objdir \"_obj/debug\"",
		# indent + "filter \"configurations:Release\"",
		# indent + indent + "objdir \"_obj/release\"",
		""
	]))
	f.close()


def create_module(root, name, cppNamespace):
	modDir = os.path.join(root, name.lower())
	if not os.path.isdir(modDir):
		os.makedirs(modDir)
	create_cpp_folder(modDir, cppNamespace, name)


def print_usage():
	print("Usage:\ncreate_module.py <moduleName> <cppNamespace=sn>")


def main():
	if len(sys.argv) >= 2:
		modName = sys.argv[1]
		cppNamespace = "sn"
		if len(sys.argv) == 3:
			cppNamespace = sys.argv[2]
		create_module("modules", modName, cppNamespace)
	else:
		print_usage()


# Place ourselves in the root of SnowfeetEngine
os.chdir('..')

main()
