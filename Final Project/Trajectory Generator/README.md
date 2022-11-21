# Component Code: Trajectory Generator

## Introduction

This part of code contains the logic of setting key points of the camera, and generating a trajectory based on the set of key points.

The class `trajGenerator` is used to set key points and generate trajectories, while the class `Utils` contains some utility functions.

## trajGenerator

Each knot of the trajectory contains the pose information of the camera. In detail, we use 3D Cartesian coordinate in the world frame to express position, and use quaternion to express orientation.

The default trajectory of the camera is a circle around the Z axis of the world frame, while the camera always faces the Z axis. To set it, please use `resetTraj` function. You can specify the radius and the height (the distance of the circle surface to the world frame origin) of the circle.

To get user-defined trajectory, first, the user need to define some key points in the world frame. Users can use `addKeyPoint`, `insertKeyPoint`, `removeKeyPoint` and `resetKeyPoints` to modify the set of key points.

After the key points are specified, users can generate the trajectory using `genTraj`. The default method to generate is linear interpolation. Some other advanced method is being developed. The speed of camera can be changed using `setVelocity`. The default speed is 1.0 pixels per frame.

The class also provide convenient functions to print key points and trajectories: `printKeyPoints` and `printTraj`.

Please view the comments in `trajGenerator.h` for more detail.

## Utils

In the file `utils.h`, we define three structs, `Position`, `Quaternion` and `Point`. We also define operator `==` for them in `utils.cpp`.

### Utilitiy function 1: `slerp`

This function implement Spherical Linear Interpolation (SLERP) of quaternions.

### Utility function 2: `quat2Euler`

This function converts a quaternion to Euler angles. The order of the Euler angles is ZYX.

### Utility function 3: `euler2Quat`

This function converts Euler angles to a quaternion. The order of the Euler angles is ZYX.

## Tests

We perform some simple tests in `Main.cpp`.

In TEST 1, we test the generation of the default trajectory.

In TEST 2, We test the process of adding, inserting, and removing key points. And then, we tried generating some trajetories and changing camera speed.