{
    "variables": {
        # Be sure to create OPENNI2 and NITE2 system vars
        "OPENNI2%": "$(OPENNI2)",
        "NITE2%": "$(NITE2)"
    },
    "targets": [
    {
        "target_name": "nuimotion",

        "sources": [
            "src/Main.cpp",
            "src/enums/EnumMapping.cpp",
            "src/gestures/GestureRecognizer.cpp",
            "src/gestures/Swipe.cpp",
            "src/gestures/Wave.cpp" ],

        "copies": [
            {
                "files": [  "<(OPENNI2)/Redist/OpenNI2/Drivers/Kinect.dll",
                            "<(OPENNI2)/Redist/OpenNI2/Drivers/OniFile.dll",
                            "<(OPENNI2)/Redist/OpenNI2/Drivers/PS1080.dll",
                            "<(OPENNI2)/Redist/OpenNI2/Drivers/PS1080.ini",
                            "<(OPENNI2)/Redist/OpenNI2/Drivers/PSLink.dll",
                            "<(OPENNI2)/Redist/OpenNI2/Drivers/PSLink.ini"],
                "destination": "<(module_root_dir)/build/Release/OpenNI2/Drivers/"
            },
            {
                "files": [  "<(NITE2)/Redist/NiTE2/Data/lbsdata.idx",
                            "<(NITE2)/Redist/NiTE2/Data/lbsdata.lbd",
                            "<(NITE2)/Redist/NiTE2/Data/lbsparam1.lbd",
                            "<(NITE2)/Redist/NiTE2/Data/lbsparam2.lbd"],
                "destination": "<(module_root_dir)/build/Release/NiTE2/Data/"
            },
            {
                "files": [  "<(NITE2)/Redist/NiTE2/FeatureExtraction.ini",
                            "<(NITE2)/Redist/NiTE2/h.dat",
                            "<(NITE2)/Redist/NiTE2/HandAlgorithms.ini",
                            "<(NITE2)/Redist/NiTE2/s.dat"],
                "destination": "<(module_root_dir)/build/Release/NiTE2/"
            },
            {
                "files": [  "<(OPENNI2)/Redist/OpenNI2.dll",
                            "<(OPENNI2)/Redist/OpenNI.ini",
                            "<(NITE2)/Redist/NiTE2.dll",
                            "<(NITE2)/Redist/NiTE.ini" ],
                "destination": "<(module_root_dir)/build/Release/"
            }
        ],
        "include_dirs": [   "./src/enums",
                            "<(OPENNI2)/Include/",
                            "<(NITE2)/Include/",
                            "<(OPENNI2)/Redist/",
                            "<(NITE2)/Redist/"],

        "libraries": ["-l<(OPENNI2)/Lib/OpenNI2", "-l<(NITE2)/Lib/NiTE2"]
    },
    {
        "target_name": "nuimotion-depth",

        "sources": [
            "src/Depth.cpp",
            "src/enums/EnumMapping.cpp",
            "src/gestures/GestureRecognizer.cpp",
            "src/gestures/Swipe.cpp",
            "src/gestures/Wave.cpp" ],

        "copies": [
            {
                "files": [  "<(OPENNI2)/Redist/OpenNI2/Drivers/Kinect.dll",
                            "<(OPENNI2)/Redist/OpenNI2/Drivers/OniFile.dll",
                            "<(OPENNI2)/Redist/OpenNI2/Drivers/PS1080.dll",
                            "<(OPENNI2)/Redist/OpenNI2/Drivers/PS1080.ini",
                            "<(OPENNI2)/Redist/OpenNI2/Drivers/PSLink.dll",
                            "<(OPENNI2)/Redist/OpenNI2/Drivers/PSLink.ini"],
                "destination": "<(module_root_dir)/build/Release/OpenNI2/Drivers/"
            },
            {
                "files": [  "<(OPENNI2)/Redist/OpenNI2.dll", "<(OPENNI2)/Redist/OpenNI2.ini"],
                "destination": "<(module_root_dir)/build/Release/"
            }
        ],
        "include_dirs": [ "<(OPENNI2)/Include/", "<(OPENNI2)/Redist/"],
        "libraries": ["-l<(OPENNI2)/Lib/OpenNI2"]
    }
  ]
}
