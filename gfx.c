/*
	Currently only opengl stuff,
	will hopefully be a decent abstraction of opengl and direct3d
	depend on how much I like direct3d
*/

//#define GL_GLEXT_PROTOTYPES
//#include "w:/lib/glext.h"

#ifdef _WIN32
#	define GLDECL WINAPI
#else
#	define GLDECL
#endif

typedef char GLchar;
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_GEOMETRY_SHADER                0x8DD9
#define GL_INFO_LOG_LENGTH                0x8B84

//typedef unsigned int GLuint;
//typedef int GLint;
//typedef unsigned int GLenum;
//typedef int GLsizei;

#define OPENGL_EXTENSION_LIST\
	GLE(GLuint, CreateShader, GLenum type)\
	GLE(void, ShaderSource, GLuint shader, GLsizei count, const GLchar * const *string, const GLint *length)\
	GLE(void, CompileShader, GLuint shader)\
	GLE(GLuint, CreateProgram, void)\
	GLE(void, AttachShader, GLuint program, GLuint shader)\
	GLE(void, LinkProgram, GLuint program)\
	GLE(void, GetShaderInfoLog, GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog)\
	GLE(void, UseProgram, GLuint program)\
	GLE(void, GetShaderiv, GLuint shader, GLenum pname, GLint *params)\
	\
	GLE(void, EnableVertexAttribArray, GLuint index)\
	GLE(void, VertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)\
	\
	GLE(GLint, GetUniformLocation, GLuint program, const GLchar *name)\
	GLE(void, UniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)\
	GLE(void, Uniform4f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)\
	GLE(GLint, GetAttribLocation_proc, GLuint program, const GLchar *name)\


#define GLE(ret, name, ...) typedef ret (GLDECL name##_proc)(__VA_ARGS__); name##_proc *gl##name;
OPENGL_EXTENSION_LIST
#undef GLE

void load_opengl_extensions() {
	//HMODULE opengl_lib = LoadLibraryA("opengl32.dll");

#define GLE(ret, name, ...) gl##name = (name##_proc*)wglGetProcAddress("gl" #name);
	OPENGL_EXTENSION_LIST
#undef GLE
}

// todo: not sure if this interface will work with direct3d
// todo: malloc? what's my stance on malloc?
char *vs_error = NULL;
char *fs_error = NULL;
char *gs_error = NULL;
GLuint shader_from_string(char *vs, char *fs, char *gs) {
	vs_error = NULL;
	fs_error = NULL;
	gs_error = NULL;

	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
	int vlen = strlen(vs);
	int flen = strlen(fs);
	glShaderSource(vshader, 1, &vs, &vlen);
	glShaderSource(fshader, 1, &fs, &flen);
	glCompileShader(vshader);
	glCompileShader(fshader);
	char buffer[256];
	glGetShaderInfoLog(vshader, 256, NULL, buffer);
	OutputDebugString(buffer);
	glGetShaderInfoLog(fshader, 256, NULL, buffer);
	OutputDebugString(buffer);

	GLuint gshader;
	if (gs) {
		gshader = glCreateShader(GL_GEOMETRY_SHADER);
		int glen = strlen(gs);
		glShaderSource(gshader, 1, &gs, &glen);
		glCompileShader(gshader);
		int gs_error_size;
		glGetShaderiv(gshader, GL_INFO_LOG_LENGTH, &gs_error_size);
		if (gs_error_size) {
			gs_error = (char*)malloc(gs_error_size);
			glGetShaderInfoLog(gshader, gs_error_size, NULL, gs_error);
			OutputDebugString(gs_error);
		}
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	if (gs) glAttachShader(program, gshader);
	glLinkProgram(program);

	return program;
}

void use_shader(GLuint shader) {
	glUseProgram(shader);
}