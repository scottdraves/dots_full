#version 150

uniform mat4 modelViewProjectionMatrix;
uniform vec2 screen;
uniform float screenScale;
uniform float maxLineLength;

in vec4 position;
in vec4 normal;
in vec4 color;
in vec2 texcoord;
in float cpPixelsPerUnit;
in vec2 cpCenter;

in float pointRadius;
in float lineWidth;

out float geomPointRadius, geomLineWidth, geomMaxLineLength;
out vec4 vertexColor;

void main(){
    geomPointRadius = pointRadius;
    geomLineWidth = lineWidth;
    geomMaxLineLength = maxLineLength;

    // Throwaways so the data gets bound, there has
    // to be a way around this, but I don't know what it is
    float x = normal.x;
    float c = color.x;
    float t = texcoord.x;

    vec2 screenCenter = screen / 2.0;
    vec2 screenPos = (position.xy - cpCenter) * screenScale * cpPixelsPerUnit + screenCenter;

    vertexColor = color;
    gl_Position = modelViewProjectionMatrix * vec4(screenPos.x, screenPos.y, 0.0, position.w);
}
