#ifndef SHADER_H
#define SHADER_H

//#include <GL/gl.h>

#include <QOpenGLShaderProgram>

#include "common.h"

class Shader : public QOpenGLShaderProgram
{
public:
  Shader(std::map<GLenum, std::string> iShaders);
  ~Shader();

  void Use();

  GLuint GetShaderName() const;

  bool IsValid() const;

  void BindUniformBufferObject(std::string iName, GLuint iBinding);

  GLuint GetUniformIndex(std::string iName);

  void UpdateUniform(std::string iName, GLfloat iValue);
  void UpdateUniform(std::string iName, GLdouble iValue);
  void UpdateUniform(std::string iName, GLuint iValue);
  void UpdateUniform(std::string iName, GLint iValue);
  void UpdateUniform(std::string iName, const QVector2D& iValue);
  void UpdateUniform(std::string iName, const QVector3D& iValue);
  void UpdateUniform(std::string iName, const QVector4D& iValue);
  void UpdateUniform(std::string iName, const QMatrix4x4& iValue);

protected:
  std::map<GLenum, GLuint> _shaders;

  GLuint _name;
  GLint _status;

  std::string _log;
};

using ShaderPtr = std::shared_ptr<Shader>;

#endif // SHADER_H
