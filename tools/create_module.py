#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, json


# mod_types = {
# 	"mod":"Mod",
# 	"sample":"Sample",
# 	"usermod":"UserMod",
# 	"tool":"Tool"
# }

header_extension = ".h"


def get_mod_lib_name(modType, name):
	return modType.title() + name.title()


def create_cpp_folder(root, modType, namespace, name):
	cppDir = os.path.join(root, "cpp")
	os.mkdir(cppDir)

	eol = "\n"
	indent = "    "

	headerName = "mod_" + name.title() + header_extension
	headerPath = os.path.join(cppDir, headerName)
	implemPath = os.path.join(cppDir, "mod_" + name.title() + ".cpp")
	premakePath = os.path.join(cppDir, "premake5.lua")

	libName = get_mod_lib_name(modType, name)
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
		indent + "return 0;"
		"}",
		"",
		unloadFunc,
		"{",
		indent + "// TODO implement",
		indent + "return 0;"
		"}",
		"",
		""
	]))
	f.close()

	f = open(premakePath, "w+")
	f.write(eol.join([
		"project \"" + get_mod_lib_name(modType, name) + "\"",
		indent + "platforms { \"x32\" }",
		indent + "commonModConfigCPP()",
		#indent + "kind \"SharedLib\"",
		#indent + "language \"C++\"",
		#indent + "dependson {",
		#indent + indent + "\"SnowfeetCore\"",
		#indent + indent + "\"JsonBox\"",
		#indent + "}",
		#indent + "location \".\"",
		#indent + "targetdir \"..\"",
		indent + "files {",
		indent + indent + "\"**" + header_extension + "\",",
		indent + indent + "\"**.cpp\"",
		indent + "}",
		#indent + "links {",
		#indent + indent + "\"SnowfeetCore\",",
		#indent + indent + "\"JsonBox\" -- TODO Don't link JsonBox, it's already done in the core (maybe a premake5 bug?)",
		#indent + "}",
		indent + "filter \"configurations:Debug\"",
		indent + indent + "objdir \"_obj/debug\"",
		#indent + indent + "defines {",
		#indent + indent + indent + "\"SN_BUILD_DEBUG\"",
		#indent + indent + "}",
		indent + "filter \"configurations:Release\"",
		indent + indent + "objdir \"_obj/release\"",
		""
	]))
	f.close()


def create_mod_file(path, modType, name):
	f = open(path, "w+")
	contents = {
		"bindings":[
			get_mod_lib_name(modType, name)
		]
	}
	f.write(json.dumps(contents, sort_keys=True, indent=4, separators=(',', ': ')))
	f.close()
	return contents


def create_module(root, modType, name, cppNamespace):
	modDir = os.path.join(root, modType + "s", name.lower())
	if not os.path.isdir(modDir):
		os.makedirs(modDir)
	
	modFilePath = os.path.join(modDir, name.lower() + ".mod.json")
	modContents = create_mod_file(modFilePath, modType, name)

	create_cpp_folder(modDir, modType, cppNamespace, name)


def print_usage():
	print("Usage:\ncreate_module.py <moduleName> <cppNamespace=sn>")


def main():
	if len(sys.argv) >= 2:
		modName = sys.argv[1]
		cppNamespace = "sn"
		if len(sys.argv) == 3:
			cppNamespace = sys.argv[2]
		create_module("projects", "mod", modName, cppNamespace)
	else:
		print_usage()


# Place ourselves in the root of SnowfeetEngine
os.chdir('..')

main()

