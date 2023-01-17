#include "GL/gl.h"

#include <string>
#include <exception>

#include <cstdarg>

// Class used for exceptions. Unlike e.g. std::runtime_error, which only
// accepts a "fixed" string, Error provides std::printf()-like formatting.
// Example:
//
//	throw Error( "glGetError() returned %d", glerr );
//
class Error : public std::exception
{
    public:
        explicit Error( char const*, ... );

    public:
        char const* what() const noexcept override;

    private:
        std::string mMsg;
};

Error::Error( char const* aFmt, ... )
{
    va_list args;
    va_start( args, aFmt );

    char buff[1024]{};
    vsnprintf( buff, 1023, aFmt, args );

    va_end( args );

    mMsg = buff;
}

char const* Error::what() const noexcept
{
    return mMsg.c_str();
}

char const* error_string_( GLenum aErr )
{
    switch( aErr )
    {
        // See
        // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetError.xhtml
        case GL_INVALID_ENUM: return "invalid enum";
        case GL_INVALID_VALUE: return "invalid value";
        case GL_INVALID_OPERATION: return "invalid operation";
        // case GL_INVALID_FRAMEBUFFER_OPERATION: return "invalid framebuffer operation";
        case GL_OUT_OF_MEMORY: return "out of memory";
        case GL_STACK_UNDERFLOW: return "underflow";
        case GL_STACK_OVERFLOW: return "overflow";
    }

    return "<unknown error value>";
}

void check_gl_error( char const* aSourceFile, int aSourceLine )
{
    auto const res = glGetError();
    if( GL_NO_ERROR != res )
    {
        throw Error( "(%s:%d) glGetError() returned %s (%d)", aSourceFile, aSourceLine, error_string_(res), res );
    }
}
