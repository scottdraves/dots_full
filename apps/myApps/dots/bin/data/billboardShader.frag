#version 150

in vec4 fragColor;
in vec2 texCoord;
in float isLine;

out vec4 outputColor;

void main() {
    if (isLine < 0.5) {
        float dist = length(texCoord - vec2(0.5, 0.5));

        if (dist > 0.5)
            discard;
    }

    outputColor = fragColor;
}
