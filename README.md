# PolyPIV
Particle Image Velocimetry software used for fluid analysis. In short, this software's purpose is to calculate the velocity map of a fluid of interest.

This award-winning software requires the following external libraries:
- FFTW for computing the discrete Fourier transform.
- FreeImage for saving images in several formats.
- GTK+ for managing the Graphic User Interface.

The GUI description was made on the GLADE software.

The development and testing environment included:

- Ubuntu Linux 16.04
- gcc version 5

#### Software Overview

![alt text](PolyPIVGUI.png)

* a) Display Area to visualize the processed images and the velocity map.
* b) Load/Save/Reset buttons.
* c) General paramenters and settings.
* d) Algorithms selection and execution.

#### Building

For building the whole project there are 2 options, once in the src folder:

- To compile the modified parts of the project: ```sh compile_all.sh ```
- To remove all the binaries and then compile the whole project: ``` sh force_compile_all.sh```

###### License

The PolyPIV software is licensed under the terms of the GPL version 3 (or later) license.

    Copyright (C) 2019 Didier Muñoz Díaz

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

See <http://www.gnu.org/licenses/> for license details.
