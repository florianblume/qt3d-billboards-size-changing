#version 150 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 pos_id;
in vec2 vertexTexCoord;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 texCoord;
flat out uint id;

uniform mat4 modelViewProjection;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
//uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 cameraPos;
uniform vec3 cameraUp;

uniform mat4 inst;  // transform of individual object instance
uniform mat4 instNormal;  // should be mat3 but Qt3D only supports mat4...

/*
mat4 billboard(vec3 position, vec3 cameraPos, vec3 cameraUp) {
    vec3 look = normalize(cameraPos - position);
    vec3 right = cross(cameraUp, look);
    vec3 up2 = cross(look, right);
    mat4 transform;
    transform[0] = vec4(right, 0);
    transform[1] = vec4(up2, 0);
    transform[2] = vec4(look, 0);
    // Uncomment this line to translate the position as well
    // (without it, it's just a rotation)
    transform[3] = vec4(position, 1);
    return transform;
}
*/

void main()
{
    vec3 pos = vec3(pos_id[0], pos_id[1], pos_id[2]);
    vec4 offsetPos = inst * vec4(vertexPosition, 1.0) + vec4(pos, 0.0);

    worldNormal = normalize(mat3(instNormal) * vertexNormal);
    worldPosition = vec3(offsetPos);
    id = uint(pos_id[3]);

    vec3 tt = vec3(vertexTexCoord, 1.0);
    texCoord = (tt / tt.z).xy;

    mat4 modelViewMatrix = viewMatrix * modelMatrix;

    // Original code
    //gl_Position = projectionMatrix * modelViewMatrix * offsetPos;

    // Doesn't work
    // Set upper left modelView matrix to identity
    // https://stackoverflow.com/questions/18048925/opengl-billboard-matrix
    // https://gamedev.stackexchange.com/questions/58245/problem-using-glmlookat/150705#150705
    // https://www.geeks3d.com/20140807/billboarding-vertex-shader-glsl/
    /*
    mat4 billboardMatrix = modelViewMatrix;

    billboardMatrix[0][0] = 1.0;
    billboardMatrix[0][1] = 0.0;
    billboardMatrix[0][2] = 0.0;

    billboardMatrix[1][0] = 0.0;
    billboardMatrix[1][1] = 1.0;
    billboardMatrix[1][2] = 0.0;

    billboardMatrix[2][0] = 0.0;
    billboardMatrix[2][1] = 0.0;
    billboardMatrix[2][2] = 1.0;

    gl_Position = projectionMatrix * billboardMatrix * offsetPos;
    */

    // Doesn't work
    //gl_Position = projectionMatrix * (modelViewMatrix * vec4(0.0, 0.0, 0.0, 1.0) + vec4(offsetPos.x, offsetPos.y, 0.0, 0.0));

    // Doesn't work
    //mat4 billboardTransform = billboard(pos, cameraPos, cameraUp);
    //gl_Position = projectionMatrix * viewMatrix * billboardTransform * offsetPos;

    //http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/billboards/
    // https://github.com/opengl-tutorials/ogl/blob/master/tutorial18_billboards_and_particles/Billboard.vertexshader
    //vec3 cameraRight = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    //gl_Position = projectionMatrix * viewMatrix * vec4(pos + cameraRight * vertexPosition.x * 100 + cameraUp * vertexPosition.y * 100, 1);

    //http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/billboards/
    //mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
    //gl_Position = viewProjectionMatrix * vec4(pos, 1.0);
    //gl_Position /= gl_Position.w;
    //gl_Position.xy += vertexPosition.xy * vec2(0.2, 0.05);

    //gl_Position = projectionMatrix * (modelViewMatrix * vec4(0.0, 0.0, 0.0, 1.0) + vec4(offsetPos.x, offsetPos.y, 0.0, 0.0));

    //https://gist.github.com/mattdesl/67838c69c22218242cadf4f5d0721d42
    /*
    float aspect = 2;
    vec2 scale = vec2(
        length(modelViewMatrix[0]) / aspect,
        length(modelViewMatrix[1])
      );

      vec4 billboard = (modelViewMatrix * vec4(vec3(0.0), 1.0));
      vec4 newPosition = projectionMatrix
        * billboard
        + vec4(scale * offsetPos.xy, 0.0, 0.0);

      gl_Position = newPosition;
      */

    //gl_Position = projectionMatrix * vec4(offsetPos.xy, 0.0, 1.0);

    // https://bugreports.qt.io/browse/QTBUG-53442
    // This produces billboards that change their size (e.g. when the camera moves away) and
    // according to the creator won't work for actual 3D objects

    // This restores the rotation of the object -> The issue wit the code above probably was
    // that our billboards are already rotated and this rotation is removed when we set the
    // upper 3x3 matrix of the modelViewMatrix to identity
    modelViewMatrix[0] = modelMatrix[0];
    //if (billBoardMode == 1)
    //modelViewMatrix[1] = modelMatrix[1]; //spherical -> looks weird but might be desirable
    modelViewMatrix[2] = modelMatrix[2];

    gl_Position = projectionMatrix * modelViewMatrix * offsetPos;
}
