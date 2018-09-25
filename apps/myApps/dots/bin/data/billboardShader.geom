#version 150

layout(lines) in;
layout(triangle_strip, max_vertices = 12) out;

uniform vec2 screen;

in vec4 vertexColor[];
in float geomPointRadius[];
in float geomLineWidth[];
in float geomMaxLineLength[];

out vec4 fragColor;
out vec2 texCoord;
out float isLine;

void main() {
    vec4 pt1 = gl_in[0].gl_Position;
    vec4 pt2 = gl_in[1].gl_Position;

    float size1 = geomPointRadius[0];
    float size2 = geomPointRadius[1];
    float lineWidth = geomLineWidth[0];
    float maxLineLength = geomMaxLineLength[0];

    vec4 ar = vec4(1.0, screen.x/screen.y, 1.0, 1.0);

    if (lineWidth > 0.001) {
        float dist = length(pt2 - pt1);
        if (dist < maxLineLength) {
            vec2 oneToTwo = (pt2-pt1).xy;
            vec2 perp = normalize(vec2(-oneToTwo.y, oneToTwo.x)) * lineWidth;

            gl_Position = pt1 + vec4(perp.x, perp.y, 0, 0) * ar;
            fragColor = vertexColor[0];
            texCoord = vec2(0, 0);
            isLine = 1;
            EmitVertex();
            gl_Position = pt1 + vec4(-perp.x, -perp.y, 0, 0) * ar;
            fragColor = vertexColor[0];
            texCoord = vec2(0, 1);
            isLine = 1;
            EmitVertex();
            gl_Position = pt2 + vec4(perp.x, perp.y, 0, 0) * ar;
            fragColor = vertexColor[1];
            texCoord = vec2(0, 1);
            isLine = 1;
            EmitVertex();
            gl_Position = pt2 + vec4(-perp.x, -perp.y, 0, 0) * ar;
            fragColor = vertexColor[1];
            texCoord = vec2(1, 1);
            isLine = 1;
            EmitVertex();
            EndPrimitive();
        }
    }

    if (size1 > 0.001) {
        gl_Position = pt1 + vec4(size1, -size1, 0, 0) * ar;
        fragColor = vertexColor[0];
        texCoord = vec2(1, 0);
        isLine = 0;
        EmitVertex();
        gl_Position = pt1 + vec4(-size1, -size1, 0, 0) * ar;
        fragColor = vertexColor[0];
        texCoord = vec2(0, 0);
        isLine = 0;
        EmitVertex();
        gl_Position = pt1 + vec4(size1, size1, 0, 0) * ar;
        fragColor = vertexColor[0];
        texCoord = vec2(1, 1);
        isLine = 0;
        EmitVertex();
        gl_Position = pt1 + vec4(-size1, size1, 0, 0) * ar;
        fragColor = vertexColor[0];
        texCoord = vec2(0, 1);
        isLine = 0;
        EmitVertex();
        EndPrimitive();
    }

    if (size2 > 0.001) {
        gl_Position = pt2 + vec4(size2, -size2, 0, 0) * ar;
        fragColor = vertexColor[1];
        texCoord = vec2(1, 0);
        isLine = 0;
        EmitVertex();
        gl_Position = pt2 + vec4(-size2, -size2, 0, 0) * ar;
        fragColor = vertexColor[1];
        texCoord = vec2(0, 0);
        isLine = 0;
        EmitVertex();
        gl_Position = pt2 + vec4(size2, size2, 0, 0) * ar;
        fragColor = vertexColor[1];
        texCoord = vec2(1, 1);
        isLine = 0;
        EmitVertex();
        gl_Position = pt2 + vec4(-size2, size2, 0, 0) * ar;
        fragColor = vertexColor[1];
        texCoord = vec2(0, 1);
        isLine = 0;
        EmitVertex();
        EndPrimitive();
    }
}