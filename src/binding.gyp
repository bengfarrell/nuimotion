{
    "conditions": [
        [
            "OS=='win'", {
                "variables": {
                    "ONI_Root%": "./../", # Set the linker location
                }
            }, { # 'OS!="win"'
                "variables": {
                    "ONI_Root%": "", # Set the linker location
                }
            }
        ]
    ],
  "targets": [
    {
        "target_name": "sweatin",
        "sources": [ "sweatin.cpp" ],
        "include_dirs": [ "./Include"],
        "libraries": ["-l<(ONI_Root)OpenNI2", "-l<(ONI_Root)NiTE2", "-Wl,-rpath ./"]
    },

    {
        "target_name": "followthehand",
        "sources": [ "followthehand.cpp" ],
        "include_dirs": [ "./Include"],
        "libraries": ["-l<(ONI_Root)OpenNI2", "-l<(ONI_Root)NiTE2", "-Wl,-rpath ./"]
    }
  ]
}
