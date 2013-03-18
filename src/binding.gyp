{
    "conditions": [
        [
            "OS=='win'", {
                "variables": {
                    # Set the linker location, no extra linking needed, just link backwards one directory
                    "ONI_Root%": "../"
                }
            }, { # 'OS!="win"'
                "variables": {
                    # Set the linker location, the -l flag is fine on Linux with no filepath
                    # However, we need the additional option -rpath for dynamic linking
                    "ONI_Root%": "",
                }
            }
        ]
    ],
    
    "targets": [
    {
        "target_name": "nuimotion",
        "sources": [ 
            "Main.cpp", 
            "enums/EnumMapping.cpp",
            "gestures/GestureRecognizer.cpp",
            "gestures/Swipe.cpp" ],
        "include_dirs": [ "./Include"],

        # -Wl,-rpath will warn of failure on Windows, don't worry - it's a Linux thing, let it fail
        "libraries": ["-l<(ONI_Root)OpenNI2", "-l<(ONI_Root)NiTE2", "-Wl,-rpath ./"]
    }
  ]
}
