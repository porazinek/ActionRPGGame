	TEMPLATE = app
	CONFIG += console
	CONFIG -= app_bundle
	CONFIG -= qt

	# Declare and assign specific variable, you may change this path to work (do not add \ at the end of path !)
	UNREAL_PATH = D:\Unreal\RPGEngine\UnrealEngine
	VISUAL_PATH = C:\Program Files (x86)\Microsoft Visual Studio 12.0

	# Will define all unreal defines
	include(defines.pri)

	# Project source path (you may complete this)
	HEADERS += Source\ActionRPGGame\*.h
	SOURCES += Source\ActionRPGGame\*.cpp

	# Project include path (you also may complete this)
	INCLUDEPATH += Source\ActionRPGGame

	# include path for generated files
	INCLUDEPATH += Intermediate\Build\Win64\Inc\ActionRPGGame

	# Will add all unreal include path 
	include(includes.pri)