#ifndef LIGHT_H
#define LIGHT_H

#include <QOpenGLShaderProgram>

#include "common.h"

struct Light {
  enum Status : unsigned int { ENABLED = 0x0001, DISABLED = 0x0000 };
};

struct DirectionalLight : public Light {
  QVector4D direction;
  QVector4D ambient;
  QVector4D diffuse;
  QVector4D specular;

  Status status = ENABLED;

  inline void UpdateToShader(std::string iPrefix,
                             QOpenGLShaderProgram& iShader) const {
    iShader.setUniformValue((iPrefix + ".direction").c_str(), direction);
    iShader.setUniformValue((iPrefix + ".ambient").c_str(), ambient);
    iShader.setUniformValue((iPrefix + ".diffuse").c_str(), diffuse);
    iShader.setUniformValue((iPrefix + ".specular").c_str(), specular);

    iShader.setUniformValue((iPrefix + ".status").c_str(),
                            (status == ENABLED) ? 1.0f : 0.0f);
  }

  void AddGUI();

 protected:
  int padding[3];
};

struct PointLight : public Light {
  QVector4D position;

  QVector4D ambient;
  QVector4D diffuse;
  QVector4D specular;

  float constant;
  float linear;
  float quadratic;

  Status status = ENABLED;

  inline void UpdateToShader(std::string iPrefix,
                             QOpenGLShaderProgram& iShader) const {
    iShader.setUniformValue((iPrefix + ".position").c_str(), position);
    iShader.setUniformValue((iPrefix + ".ambient").c_str(), ambient);
    iShader.setUniformValue((iPrefix + ".diffuse").c_str(), diffuse);
    iShader.setUniformValue((iPrefix + ".specular").c_str(), specular);

    iShader.setUniformValue((iPrefix + ".constant").c_str(), constant);
    iShader.setUniformValue((iPrefix + ".linear").c_str(), linear);
    iShader.setUniformValue((iPrefix + ".quadratic").c_str(), quadratic);

    iShader.setUniformValue((iPrefix + ".status").c_str(),
                            (status == ENABLED) ? 1.0f : 0.0f);
  }

  void AddGUI();
};

struct SpotLight : public Light {
  QVector4D position;
  QVector4D direction;
  QVector4D ambient;
  QVector4D diffuse;
  QVector4D specular;

  float constant;
  float linear;
  float quadratic;

  float cutOff;
  float outerCutOff;

  Status status = ENABLED;

  inline void UpdateToShader(std::string iPrefix,
                             QOpenGLShaderProgram& iShader) const {
    iShader.setUniformValue((iPrefix + ".position").c_str(), position);
    iShader.setUniformValue((iPrefix + ".direction").c_str(), direction);
    iShader.setUniformValue((iPrefix + ".ambient").c_str(), ambient);
    iShader.setUniformValue((iPrefix + ".diffuse").c_str(), diffuse);
    iShader.setUniformValue((iPrefix + ".specular").c_str(), specular);

    iShader.setUniformValue((iPrefix + ".constant").c_str(), constant);
    iShader.setUniformValue((iPrefix + ".linear").c_str(), linear);
    iShader.setUniformValue((iPrefix + ".quadratic").c_str(), quadratic);
    iShader.setUniformValue((iPrefix + ".cutOff").c_str(), cutOff);
    iShader.setUniformValue((iPrefix + ".outerCutOff").c_str(), outerCutOff);

    iShader.setUniformValue((iPrefix + ".status").c_str(),
                            (status == ENABLED) ? 1.0f : 0.0f);
  }

  void AddGUI();

 protected:
  int padding[2];
};

#endif  // LIGHT_H
