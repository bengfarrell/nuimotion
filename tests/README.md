If running tests from this folder as the working directory, please ensure that both the
NiTE2 and OpenNI2 folders are available in this folder alongside the test scripts.

Will also work, if the working directory is your project, contain the folders, and the tests are run from there.

Instructions on how to find these files are found in this Node Add-Ons main README.md

Example #1:
===========

Working Directory is "myproject/node_modules/nuimotion/tests"

Then, tests must contain:

events.js (if this is the node file you choose to run)

skeleton.js (if this is the node file you choose to run)

NiTE2 folder

OpenNI2 folder

And the Node file you run will be one of the JS files in "tests".


Example #2:
===========

Working Directory is "myproject"

The "myproject" must contain:

yourMainNodejsFile.js

NiTE2 folder

OpenNI2 folder

And the Node file you run will be yourMainNodejsFile.js