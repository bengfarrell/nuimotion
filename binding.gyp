{
    "variables": {
	# Be sure to create OPENNI2 and NITE2 system vars
        "OPENNI2%": "$(OPENNI2)",
        "NITE2%": "$(NITE2)"
    },

    "targets": [
    {
    	"target_name":"copy-files",

    	"conditions": [
			[ "OS=='win'", {
				"copies": [
			    	{ 	"files": [  "<(OPENNI2)/Redist/OpenNI2/Drivers/Kinect.dll",
				            		"<(OPENNI2)/Redist/OpenNI2/Drivers/OniFile.dll",
				            		"<(OPENNI2)/Redist/OpenNI2/Drivers/PS1080.dll",
				            		"<(OPENNI2)/Redist/OpenNI2/Drivers/PS1080.ini",
				            		"<(OPENNI2)/Redist/OpenNI2/Drivers/PSLink.dll",
				            		"<(OPENNI2)/Redist/OpenNI2/Drivers/PSLink.ini"],
					"destination": "<(module_root_dir)/build/Release/OpenNI2/Drivers/"
			    	},

			    	# If NITE folder is not placed at root of project, it cannot be accessed
			    	# go up through node_modules to project root and drop in NiTE2 folder
			    	{ 	"files": [  "<(NITE2)/Redist/NiTE2/Data/lbsdata.idx",
				            		"<(NITE2)/Redist/NiTE2/Data/lbsdata.lbd",
				            		"<(NITE2)/Redist/NiTE2/Data/lbsparam1.lbd",
				            		"<(NITE2)/Redist/NiTE2/Data/lbsparam2.lbd"],
						"destination": "<(module_root_dir)/../../NiTE2/Data/"
			    	},
			    	{	"files": [  "<(NITE2)/Redist/NiTE2/FeatureExtraction.ini",
				            		"<(NITE2)/Redist/NiTE2/h.dat",
				            		"<(NITE2)/Redist/NiTE2/HandAlgorithms.ini",
				            		"<(NITE2)/Redist/NiTE2/s.dat"],
						"destination": "<(module_root_dir)/../../NiTE2/"
			    	},
			    	{	"files": [  "<(OPENNI2)/Redist/OpenNI2.dll",
				            	"<(OPENNI2)/Redist/OpenNI.ini",
				            	"<(NITE2)/Redist/NiTE2.dll",
				            	"<(NITE2)/Redist/NiTE.ini" ],
						"destination": "<(module_root_dir)/build/Release/"
			    	}
				],
        		"libraries": ["-l<(OPENNI2)/Lib/OpenNI2", "-l<(NITE2)/Lib/NiTE2"]
        	}],

		    ["OS=='mac'", {
				"copies": [   
					{ 	"files": [  "<(OPENNI2)/Redist/OpenNI2/Drivers/libOniFile.dylib",
					            	"<(OPENNI2)/Redist/OpenNI2/Drivers/libPS1080.dylib",
					            	"<(OPENNI2)/Redist/OpenNI2/Drivers/PS1080.ini",
					            	"<(OPENNI2)/Redist/OpenNI2/Drivers/PSLink.ini"],
						"destination": "<(module_root_dir)/build/Release/OpenNI2/Drivers/"
				    },

				    # If NITE folder is not placed at root of project, it cannot be accessed
				    # go up through node_modules to project root and drop in NiTE2 folder
				    
					{ 	"files": [  "<(NITE2)/Redist/NiTE2/Data/lbsdata.idx",
					            	"<(NITE2)/Redist/NiTE2/Data/lbsdata.lbd",
					            	"<(NITE2)/Redist/NiTE2/Data/lbsparam1.lbd",
					            	"<(NITE2)/Redist/NiTE2/Data/lbsparam2.lbd"],
						"destination": "<(module_root_dir)/../../NiTE2/Data/"
				    },
				    
					{ 	"files": [  "<(NITE2)/Redist/NiTE2/FeatureExtraction.ini",
					            	"<(NITE2)/Redist/NiTE2/h.dat",
					            	"<(NITE2)/Redist/NiTE2/HandAlgorithms.ini",
					            	"<(NITE2)/Redist/NiTE2/s.dat"],
						"destination": "<(module_root_dir)/../../NiTE2/"
				    },
				    
					{ 	"files": [  "<(OPENNI2)/Redist/libOpenNI2.dylib",
					            	"<(OPENNI2)/Redist/OpenNI.ini",
					            	"<(NITE2)/Redist/libNiTE2.dylib",
					            	"<(NITE2)/Redist/NiTE.ini" ],
						"destination": "<(module_root_dir)/build/Release/"
				    }
				]
		    }],

		    ["OS=='linux'", {
				"copies": [   
					{ 	"files": [  "<(OPENNI2)/Redist/OpenNI2/Drivers/libOniFile.so",
					            	"<(OPENNI2)/Redist/OpenNI2/Drivers/libPS1080.so",
					            	"<(OPENNI2)/Redist/OpenNI2/Drivers/PS1080.ini",
					            	"<(OPENNI2)/Redist/OpenNI2/Drivers/PSLink.ini"],
						"destination": "<(module_root_dir)/build/Release/OpenNI2/Drivers/"
				    },

				    # If NITE folder is not placed at root of project, it cannot be accessed
				    # go up through node_modules to project root and drop in NiTE2 folder
				    
					{ 	"files": [  "<(NITE2)/Redist/NiTE2/Data/lbsdata.idx",
					            	"<(NITE2)/Redist/NiTE2/Data/lbsdata.lbd",
					            	"<(NITE2)/Redist/NiTE2/Data/lbsparam1.lbd",
					            	"<(NITE2)/Redist/NiTE2/Data/lbsparam2.lbd"],
						"destination": "<(module_root_dir)/../../NiTE2/Data/"
				    },
				    
					{ 	"files": [  "<(NITE2)/Redist/NiTE2/FeatureExtraction.ini",
					            	"<(NITE2)/Redist/NiTE2/h.dat",
					            	"<(NITE2)/Redist/NiTE2/HandAlgorithms.ini",
					            	"<(NITE2)/Redist/NiTE2/s.dat"],
						"destination": "<(module_root_dir)/../../NiTE2/"
				    },
				    
					{ 	"files": [  "<(OPENNI2)/Redist/libOpenNI2.so",
					            	"<(OPENNI2)/Redist/OpenNI.ini",
					            	"<(NITE2)/Redist/libNiTE2.so",
					            	"<(NITE2)/Redist/NiTE.ini" ],
						"destination": "<(module_root_dir)/build/Release/"
				    }
				]
		    }]
		]
    },

    {
        "target_name": "nuimotion",

        "sources": [
            "src/Main.cpp",
            "src/enums/EnumMapping.cpp",
            "src/gestures/GestureRecognizer.cpp",
            "src/gestures/Swipe.cpp",
            "src/gestures/Wave.cpp" ],

		"conditions": [
			[ "OS=='win'", {
        		"libraries": ["-l<(OPENNI2)/Lib/OpenNI2", "-l<(NITE2)/Lib/NiTE2"]
        	}],

		    ["OS=='mac'", {
		        "libraries": ["<(OPENNI2)/Tools/libOpenNI2.dylib", "<(NITE2)/Redist/libNiTE2.dylib"]
		    }],
		    ["OS=='linux'", {
		        "libraries": ["<(OPENNI2)/Tools/libOpenNI2.so", "<(NITE2)/Redist/libNiTE2.so"]
		    }],
	 	],

    	"include_dirs": [ "./src/enums", "./build/Release", "<(OPENNI2)/Include/", "<(NITE2)/Include/" ],
    },

    {
        "target_name": "nuimotion-depth",

        "sources": [
            "src/Depth.cpp",
            "src/enums/EnumMapping.cpp",
            "src/gestures/GestureRecognizer.cpp",
            "src/gestures/Swipe.cpp",
            "src/gestures/Wave.cpp" ],

		"conditions": [
			[ "OS=='win'", {
        		"libraries": ["-l<(OPENNI2)/Lib/OpenNI2"]
        	}],

		    ["OS=='mac'", {
		        "libraries": ["<(OPENNI2)/Tools/libOpenNI2.dylib"]
		    }],

		    ["OS=='linux'", {
		        "libraries": ["<(OPENNI2)/Tools/libOpenNI2.so"]
		    }],
	 	],

        "include_dirs": [ "<(OPENNI2)/Include/"]
    }
  ]
}
