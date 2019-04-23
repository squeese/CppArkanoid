#include "program.h"

/*
class BasicVS : public Shader<GL_VERTEX_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in mat4 mModel;

    uniform mat4 mP;
    uniform mat4 mV;
    uniform vec3 ambient;

    out vec4 color;

    void main() {
      color = vec4(vec3(0.5, 0.5, 0.5) * ambient, 1.0);
      gl_Position = mP * mV * mModel * vec4(aPos, 1.0);
    }
  )";
};

class BasicVS2 : public Shader<GL_VERTEX_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    // layout (location = 1) in mat4 mModel;
    uniform mat4 mP;
    uniform mat4 mV;
    uniform mat4 mModel;
    out vec3 color;
    void main() {
      color = aPos;
      gl_Position = mP * mV * mModel * vec4(aPos, 1.0);
    }
  )";
};

// layout (location = 1) in vec3 aCol;
//color = aCol;

class BasicFS : public Shader<GL_FRAGMENT_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    in vec4 color;
    out vec4 FragColor;
    void main() {
      FragColor = color;
    }
  )";
};

class MeshVertexShader : public Shader<GL_VERTEX_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 150 core
    in vec3 vPosition;  // The vertex in the LOCAL coordinate system
    uniform mat4 mM;    // The matrix for the pose of the model
    uniform mat4 mV;    // The matrix for the pose of the camera
    uniform mat4 mP;    // The matrix for 3d->2d projection
    uniform float fTime;
    out vec4 color;
    void main() {
      vec4 pos0 = mV * mM * vec4(vPosition, 1.0);
      gl_Position = mP * pos0;
      float r = 0.5 * sin(2 * fTime) + 0.5;
      float g = 0.4 * cos(3 * (pos0.y * pos0.y * pos0.y)) + 0.6;
      float b = 0.4 * sin(3 * (pos0.x * pos0.x * pos0.x)) + 0.6;
      float a = 1 / (pos0.z / -1.75);
      color = vec4(r, g, b, clamp(a*a*a, 0.1, 1));
    }
  )";
};

class MeshFragmentShader : public Shader<GL_FRAGMENT_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 150 core
    in vec4 color;
    out vec4 outColor;
    void main() {
      outColor = color;
    }
  )";
};

class BloomVS : public Shader<GL_VERTEX_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec2 aTexCoords;

    out VS_OUT {
      vec3 FragPos;
      vec3 Normal;
      vec2 TexCoords;
    } vs_out;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    void main() {
      vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
      vs_out.TexCoords = aTexCoords;
      mat3 normalMatrix = transpose(inverse(mat3(model)));
      vs_out.Normal = normalize(normalMatrix * aNormal);
      gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
  )";
};

class BloomFS : public Shader<GL_FRAGMENT_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    layout (location = 0) out vec4 FragColor;
    layout (location = 1) out vec4 BrightColor;

    in VS_OUT {
      vec3 FragPos;
      vec3 Normal;
      vec2 TexCoords;
    } fs_in;

    struct Light {
      vec3 Position;
      vec3 Color;
    };

    uniform Light lights[4];
    uniform sampler2D diffuseTexture;
    uniform vec3 viewPos;

    void main() {
      vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
      vec3 normal = normalize(fs_in.Normal);
      vec3 ambient = 0.0 * color;
      vec3 lighting = vec3(0.0);
      vec3 viewDir = normalize(viewPos - fs_in.FragPos);
      for(int i = 0; i < 4; i++) {
        vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 result = lights[i].Color * diff * color;
        float distance = length(fs_in.FragPos - lights[i].Position);
        result *= 1.0 / (distance * distance);
        lighting += result;
      }
      vec3 result = ambient + lighting;
      float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
      if(brightness > 1.0)
        BrightColor = vec4(result, 1.0);
      else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
      FragColor = vec4(result, 1.0);
    }
  )";
};

class LightBoxFS : public Shader<GL_FRAGMENT_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    layout (location = 0) out vec4 FragColor;
    layout (location = 1) out vec4 BrightColor;

    in VS_OUT {
        vec3 FragPos;
        vec3 Normal;
        vec2 TexCoords;
    } fs_in;

    uniform vec3 lightColor;

    void main() {
      FragColor = vec4(lightColor, 1.0);
      float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
      if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
      else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
  )";
};

class BlurVS : public Shader<GL_VERTEX_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoords;

    out vec2 TexCoords;

    void main() {
      TexCoords = aTexCoords;
      gl_Position = vec4(aPos, 1.0);
    }
  )";
};

class BlurFS : public Shader<GL_FRAGMENT_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoords;

    uniform sampler2D image;

    uniform bool horizontal;
    uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

    void main() {
      vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
      vec3 result = texture(image, TexCoords).rgb * weight[0];
      if(horizontal) {
        for(int i = 1; i < 5; ++i) {
         result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
         result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
      } else {
        for(int i = 1; i < 5; ++i) {
          result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
          result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
      }
      FragColor = vec4(result, 1.0);
    }
  )";
};

class FinalVS : public Shader<GL_VERTEX_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoords;

    out vec2 TexCoords;

    void main() {
      TexCoords = aTexCoords;
      gl_Position = vec4(aPos, 1.0);
    }
  )";
};

class FinalFS : public Shader<GL_FRAGMENT_SHADER> {
public:
  inline static const char *SOURCE = R"(
    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoords;

    uniform sampler2D scene;
    uniform sampler2D bloomBlur;
    uniform bool bloom;
    uniform float exposure;

    void main() {
      const float gamma = 2.2;
      vec3 hdrColor = texture(scene, TexCoords).rgb;
      vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
      if(bloom)
        hdrColor += bloomColor; // additive blending
      vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
      result = pow(result, vec3(1.0 / gamma));
      FragColor = vec4(result, 1.0);
    }
  )";
};
*/
