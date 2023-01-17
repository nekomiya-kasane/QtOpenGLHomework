#include "shader.h"

//Shader::Shader(GLenum iType, std::string iPath, bool iIsInplace /* = true */)
//{
//  if (!iIsInplace)
//  {
//    std::ifstream file(iPath);
//    if (!file.is_open())
//    {
//      _log = "Failed to open shader file.";
//      return;
//    }
//    std::stringstream buffer;
//    buffer << file.rdbuf();
//    iPath = buffer.str();
//  }

//  _name = glCreateShader(iType);

//  OGL_CHECKPOINT_ALWAYS();

//  GLchar const *files[] = {iPath.c_str()};
//  glShaderSource(_name, 1, files, NULL);
//  glCompileShader(_name);

//  glGetShaderiv(_name, GL_COMPILE_STATUS, &_status);

//  GLchar log[512];
//  if (!_status)
//  {
//    glGetShaderInfoLog(_name, 512, NULL, log);
//    _log = log;
//#ifdef _DEBUG
//    std::cout << _log << std::endl;
//#endif
//  }
//}

//Shader::~Shader()
//{
//  glDeleteShader(_name);
//}

//GLuint Shader::GetShaderName() const
//{
//  return _name;
//}

//const std::string &Shader::GetLog() const
//{
//  return _log;
//}

//bool Shader::IsValid() const
//{
//  return !!_status;
//}

// Shader Program

Shader::Shader(std::map<GLenum, std::string> iShaders) : _status(0)
{
  _name = glCreateProgram();

  OGL_CHECKPOINT_ALWAYS();

  for (const auto &shader : _shaders)
  {
    auto id = shader->GetShaderName();
    glAttachShader(_name, id);
  }

  OGL_CHECKPOINT_ALWAYS();

  glLinkProgram(_name);
  OGL_CHECKPOINT_ALWAYS();
  glGetProgramiv(_name, GL_LINK_STATUS, &_status);

  if (_status)
    return;

  GLchar log[512];
  glGetProgramInfoLog(_name, 512, NULL, log);
  _log = log;

#ifdef _DEBUG
  std::cout << _log << std::endl;
#endif
}

Shader::~Shader()
{
  for (auto &shader : _shaders)
  {
    glDetachShader(_name, shader->GetShaderName());
  }
  glDeleteProgram(_name);
}

void Shader::Use()
{
  glUseProgram(_name);
}

GLuint Shader::GetShaderName() const
{
  return _name;
}

const std::string &Shader::GetLog() const
{
  return _log;
}

bool Shader::IsValid() const
{
  return !!_status;
}

void Shader::BindUniformBufferObject(std::string iName, GLuint iBinding)
{
  GLuint index = glGetUniformBlockIndex(_name, iName.c_str());
  if (index != GL_INVALID_INDEX)
  {
    glUniformBlockBinding(_name, index, iBinding);
  }
  // Program Uniform --> Binding Point <-- Buffer
  //                  ��
  else
  {
#ifdef _DEBUG
    std::cout << "Failed to bind uniform buffer object." << std::endl;
#endif
  }
}

void Shader::BindShaderStorageBufferObject(std::string iName, GLuint iBinding)
{
  GLuint index = glGetProgramResourceIndex(_name, GL_SHADER_STORAGE_BLOCK, iName.c_str());
  if (index != GL_INVALID_INDEX)
  {
    glShaderStorageBlockBinding(_name, index, iBinding);
  }
  // Program Uniform --> Binding Point <-- Buffer
  //                  ��
  else
  {
#ifdef _DEBUG
    std::cout << "Failed to bind shader storage buffer object." << std::endl;
#endif
  }
}

GLuint Shader::GetUniformIndex(std::string iName)
{
  auto index = glGetUniformLocation(_name, iName.c_str());
  if (index == GL_INVALID_INDEX)
  {
    // std::cout << "Failed to get uniform location: " << iName << std::endl;
  }
  return index;
}

void Shader::UpdateUniform(std::string iName, GLfloat iValue)
{
  auto index = GetUniformIndex(iName);
  glUniform1f(index, iValue);
}

void Shader::UpdateUniform(std::string iName, GLdouble iValue)
{
  auto index = GetUniformIndex(iName);
  glUniform1d(index, iValue);
}

void Shader::UpdateUniform(std::string iName, GLint iValue)
{
  auto index = GetUniformIndex(iName);
  glUniform1i(index, iValue);
}

void Shader::UpdateUniform(std::string iName, GLuint iValue)
{
  auto index = GetUniformIndex(iName);
  glUniform1ui(index, iValue);
}

void Shader::UpdateUniform(std::string iName, Vec2f iValue)
{
  auto index = GetUniformIndex(iName);
  glUniform2f(index, iValue.x, iValue.y);
}

void Shader::UpdateUniform(std::string iName, Vec3f iValue)
{
  auto index = GetUniformIndex(iName);
  glUniform3f(index, iValue.x, iValue.y, iValue.z);
}

void Shader::UpdateUniform(std::string iName, Vec4f iValue)
{
  auto index = GetUniformIndex(iName);
  glUniform4f(index, iValue.x, iValue.y, iValue.z, iValue.w);
}

void Shader::UpdateUniform(std::string iName, Mat44f iValue)
{
  auto index = GetUniformIndex(iName);
  glUniformMatrix4fv(index, 1, GL_TRUE, iValue.v);
}
