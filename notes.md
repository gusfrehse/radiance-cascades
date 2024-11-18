# NOTES

## textures
### JFA
JFA needs an input image, colored pixels are objects.
JFA outputs an image, with the closest distance.
So one input image and one output.

This is wrong. The JFA shader will do ONE jfa step, outputting the result of that step. So we need atleast two buffers for JFA.

    input -> jfa 0
    jfa 0 -> jfa 1
    ...
    jfa log(n) - 1 -> jfa log(n)

We can just swap the buffers every draw (jfa step).

So we create a framebuffer, and draw with our with the input as a sampler. The render output will be the distance field. And will be stored as the framebuffer.

Then we will have the distance field as a texture. By now we will render it to a fullscreen quad.

### Cascades
Cascades is more complex, will check later :P

### Table
| Texture | What           |
|---------|----------------|
| 0       | JFA even/input |  
| 1       | JFA odd        |
| 2       | ...            |