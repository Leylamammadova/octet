///////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// Single texture shader with no lighting

namespace octet {
	namespace shaders {
		class invert_shader : public shader {
			
			GLuint modelToProjectionIndex_;
			GLuint samplerIndex_;
		public:
			void init() {
#
				const char vertex_shader[] = SHADER_STR(
					varying vec2 uv_;

				attribute vec4 pos;
				attribute vec2 uv;

				uniform mat4 modelToProjection;

				void main() { gl_Position = modelToProjection * pos; uv_ = uv; }
				);

				const char fragment_shader[] = SHADER_STR(
					varying vec2 uv_;
				uniform sampler2D sampler;

				uniform sampler2D texture;
				void main() {
					//http://xorshaders.weebly.com/tutorials/category/beginner

					vec4 Color = texture2D(sampler, uv_);
					gl_FragColor = vec4(vec3(1.0) - Color.rgb, Color.a);//invert shader


				}
				);

	
				shader::init(vertex_shader, fragment_shader);

				modelToProjectionIndex_ = glGetUniformLocation(program(), "modelToProjection");
				samplerIndex_ = glGetUniformLocation(program(), "sampler");
			}

			void render(const mat4t &modelToProjection, int sampler) {
			
				shader::render();

				glUniform1i(samplerIndex_, sampler);
				glUniformMatrix4fv(modelToProjectionIndex_, 1, GL_FALSE, modelToProjection.get());
			}
		};
	}
}
