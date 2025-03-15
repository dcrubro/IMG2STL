# IMG2STL

**IMG2STL** is a simple tool, used for converting images into STL files. They can then be used for 3D printing or as normal models.

## How to use
**IMG2STL** is a CLI tool, thus it must be ran in a terminal.\
From within the directory of the executable, run the following command:

#### On Windows
```
.\IMG2STL.exe C:\Path\To\Image.png C:\Path\To\Output.stl <minimum_height> <maximum_height> <scale_factor> <minimum_thickness>
```

#### On Linux/macOS
```
./IMG2STL /path/to/Image.png /path/to/Output.stl <minimum_height> <maximum_height> <scale_factor> <minimum_thickness>
```

#### Argument description

**Minimum height:** The height of the least extruded (black) points (Relative to the min. thickness).\
**Maximum height:** The height of the most extruded (white) points (Relative to the min. thickness).\
**Scale factor:** 0 to 1 decimal number of the scaling factor.\
**Minimum thickness:** In millimetres, the minimum height of any point. Used for solidifying the object.

*Note: output object may be rotated incorrectly. Make changes to output file manually.*

## How it works
**IMG2STL** reads the individual pixels in an image, and generates a plane of triangles based on the color of each pixel, black being the least and white being the most extruded. The extrusion is bound by an upper and lower limit, specified by the user.

This results in a highly detailed plane with potentially millions of triangles (this plane needs to be simplified to be practically possible - WIP). The plane is then filled in at the bottom with a minimum thickness, to make the object solid. 

### Miscellaneous
Made by DcruBro, 2025. Licensed under GPLv3.\
[dcrubro.com](https://www.dcrubro.com/)\
By using this software you are agreeing to the terms of the GPLv3 license.\
This software is provided under no warranty of any kind. Use at your own risk.