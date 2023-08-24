# MyRayTracer

## Description

This is a naive ray tracer following shirley's Ray Tracing Series.  

## Prerequisites

g++

## Running

Compiling

```
g++ main.cpp -o shading -O3
```

And rendering

```
./shading
```

## Rendered pics

Some results (vary by the code):

Global Illumination VS Local Illumination (Blue ball rendered with Phong illumination)

Sampling less for faster rendering, which leads to black spots

![Image text](https://github.com/AlcEccentric/MyRayTracer/blob/master/shadingWorld.png)

Perlin Noise

![Image text](https://github.com/AlcEccentric/MyRayTracer/blob/master/perlinNoise.png)

Different Balls

![Image text](https://github.com/AlcEccentric/MyRayTracer/blob/master/checkerAndMotion.png)

## Authors

* **AlcEccentric** - *Initial work*

## Acknowledgments

* Sean Barrett for his stb_image.h in https://github.com/nothings/stb/blob/master/stb_image.h

* Peter Shirley's *Realistic Ray Tracing*
