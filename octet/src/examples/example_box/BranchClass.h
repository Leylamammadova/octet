#pragma once

namespace octet{
class branch {
  mat4t modelToWorld;
  float branchLength;
  vec4 colour;

public:
  void init(mat4t mat, vec4 branchColour, float length) {
    modelToWorld = mat;
    colour = branchColour;
    branchLength = length;
  }

  void render(color_shader&shader, mat4t &cameraToWorld) {
    mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

    shader.render(modelToProjection, colour);

    float points[] = {
      0, 0, 0,
      0, branchLength, 0,
    };

    glVertexAttribPointer(attribute_pos, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)points);
    glEnableVertexAttribArray(attribute_pos);

    glDrawArrays(GL_LINES, 0, 2);
  }
};
}