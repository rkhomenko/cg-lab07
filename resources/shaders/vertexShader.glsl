// Computer graphic lab 7
// Variant 20
// Copyright © 2017-2018 Roman Khomenko (8O-308)
// All rights reserved

#version 330 core

attribute highp vec2 position;

uniform highp mat4x4 transformMatrix;

void main() {
    gl_Position = vec4(position.xy, 0, 1) * transformMatrix;
}
