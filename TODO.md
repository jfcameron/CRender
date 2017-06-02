//ALL
convert all printf DEBUG to a new function, printTodebugLog, variable definition based on platform.
Move GL includes to an H file. Dont repeat all that precompiler logic everywhere.


Android: rename program entry point to Main.
make use of GRAPHICS* in Global.h

Move android updates to windows project, fix any issues.
Move code to a multi IDE project

Android: fix time definition
Android: fix screen resolution getter
Windows: fix screen resolution getter

//ANDROID
Consolidate Android entry and windows entry file. Separate files doesnt make sense!!
Consolidate resource files, textures etc.
Figure out how to load resources from the common directory when running from ide.
Add a SCREEN X,Y and updateScreenSize() to graphics.c
Textures should be read from /Textures not ROOT

Windows: main.c to main.cpp