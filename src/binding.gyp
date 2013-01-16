{
  "targets": [
    {
        "target_name": "sweatin",
        "sources": [ "sweatin.cpp" ],
        "include_dirs": [ "./Include"],
        "libraries": ["-lOpenNI2", "-lNiTE2", "-Wl,-rpath ./"]
    },

    {
        "target_name": "followthehand",
        "sources": [ "followthehand.cpp" ],
        "include_dirs": [ "./Include"],
        "libraries": ["-lOpenNI2", "-lNiTE2", "-Wl,-rpath ./"]
    }
  ]
}
