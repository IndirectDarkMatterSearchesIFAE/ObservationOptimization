## Purpose

Code that computes the optimal integration angle (size of region of interest) and wobble distance (pointing direction of the telescope with respect to the actual source position) for Imaging Atmospheric Cherenkov Telescopes (IACTs). It requires instrumental inputs such as the off-center acceptance and information about the target, specially the astrophysical factor in function of the radial angle with uncertainties.

## Installation
Codes is temporary and requires major overhaul. In the moment you can either `git clone` it or download it and run it. Source code (change only paths) is stored in `source` while useful functions are stored in `macros`. Some inputs are stored in `references`

## Dependencies
None

## Usage and philosophy
In the following we describe the basic steps to be done together with some methodology description. The optimization takes into account the following considerations
* the dark matter profile is peculiar, it is peaked at the source center and degrades more or less rapidly according to different models. On the other hand, the background rate is flat over the field of view. It is therefore required to optimized the signal region
* the acceptance of the camera changes in function of the distance from the center, and this differs for different instruments
* signal can leak into the background control region. This can be avoided by optimizing the distance of the background control region, defined by the s wobble parameters, but leakage must be computed anyhow
* any direction in the sky must be smeared with the telescope point spread function

### Define instruments settings
We need the following instrumental settings
* acceptance in function of the offset from camera center
* point spread function

Some informations are known for existing instruments such as MAGIC (because they are published), such as CTA (because they are produced). Further instruments can be input manually. This is done with the `source` classes 
```
source/JDInstruments.h
source/JDInstruments.cc
```
and with the functions script
```
macros/examplesJDInstruments.cxx
```

