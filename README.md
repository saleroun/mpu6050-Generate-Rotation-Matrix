# imu-Rotation Matrix Creation

You usually have to align your sensor with your inertial reference frame to be able to interpret the data.
you can find both header files in https://github.com/jrowberg/i2cdevlib
The way to do this is with rotation matrices whose individual elements are populated with trigonometric functions or quaternions. Quaternions are preferred because they require less computation by an MCU.
this matrix capture raw data from the initial mpu situation, after calculation run final-data().
this matrix not only support mpu6050 but after some changes you can use in other types of imu.
Whichever sensor you use, you must calibrate the device according to the manufacturer's datasheet prior to every use. Sometimes that calibration requires an external EEPROM.

# Rotation Matrices
Moving a vector around in three-dimensional space can be a complicated affair. One common technique uses sequential rotations around fixed axes to rotate vectors.
The easiest way to reorient your vectors in a single rotation is with a rotation matrix. A 3×3 matrix contains all of the necessary information to move a vector in a single rotation without using trigonometry.
You can learn more about that technique by searching for “Euler angles” or by reading my previous article on quaternions.
In the gifs below, you can see sequential rotations for yaw, pitch, and roll, each of which requires a specific orientation of axes for proper function. Whenever a single movement can be described by multiple rotations, gimbal-lock is possible.
A three-dimensional vector that lies on the unit sphere can be rotated into any new position with a single rotation about a fixed axis. It is much faster and only requires the basic calculations of add, subtract, divide, and multiply.
 
