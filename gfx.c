/*
	Currently only opengl stuff,
	will hopefully be a decent abstraction of opengl and direct3d
	depend on how much I like direct3d
*/

//#define GL_GLEXT_PROTOTYPES
//#include "w:/lib/glext.h"

#pragma warning(disable: 4838)

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
#define GL_MULTISAMPLE                    0x809D
#define GL_MULTISAMPLE_ARB                0x809D

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
	GLE(void, DeleteProgram, GLuint program)\
	\
	GLE(void, EnableVertexAttribArray, GLuint index)\
	GLE(void, VertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)\
	\
	GLE(GLint, GetUniformLocation, GLuint program, const GLchar *name)\
	GLE(void, UniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)\
	GLE(void, Uniform1f, GLint location, GLfloat v0)\
	GLE(void, Uniform2f, GLint location, GLfloat v0, GLfloat v1)\
	GLE(void, Uniform3f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)\
	GLE(void, Uniform4f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)\
	GLE(GLint, GetAttribLocation, GLuint program, const GLchar *name)\


#define GLE(ret, name, ...) typedef ret (GLDECL name##_proc)(__VA_ARGS__); name##_proc *gl##name;
OPENGL_EXTENSION_LIST
#undef GLE

void load_opengl_extensions() {
	//HMODULE opengl_lib = LoadLibraryA("opengl32.dll");

#define GLE(ret, name, ...) gl##name = (name##_proc*)wglGetProcAddress("gl" #name);
	OPENGL_EXTENSION_LIST
#undef GLE
}

typedef struct {
	GLuint gl_program;
	char *file_name;
	//HANDLE file_handle;
	uint64_t last_write_time;
	int types;
} Shader;

// todo: not sure if this interface will work with direct3d
// todo: malloc? what's my stance on malloc?
char *vs_error = NULL;
char *fs_error = NULL;
char *gs_error = NULL;

GLuint create_gl_shader(char *source, int len, GLenum type) {
	GLuint shader = glCreateShader(type);
	//int glen = strlen(source);
	glShaderSource(shader, 1, &source, &len);
	glCompileShader(shader);
	int error_size;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &error_size);
	if (error_size) {
		char *error_out;
		if (type == GL_VERTEX_SHADER) error_out = vs_error;
		if (type == GL_FRAGMENT_SHADER) error_out = fs_error;
		if (type == GL_GEOMETRY_SHADER) error_out = gs_error;
		error_out = (char*)malloc(error_size);
		glGetShaderInfoLog(shader, error_size, NULL, error_out);
		OutputDebugString(error_out);
	}
	return shader;
}

Shader shader_from_string(char *vs, char *fs, char *gs) {
	vs_error = NULL;
	fs_error = NULL;
	gs_error = NULL;

	/*GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
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
	}*/

	GLuint program = glCreateProgram();
	if (vs) {
		GLuint shader = create_gl_shader(vs, strlen(vs), GL_VERTEX_SHADER);
		glAttachShader(program, shader);
	}
	if (fs) {
		GLuint shader = create_gl_shader(fs, strlen(fs), GL_FRAGMENT_SHADER);
		glAttachShader(program, shader);
	}
	if (gs) {
		GLuint shader = create_gl_shader(gs, strlen(gs), GL_GEOMETRY_SHADER);
		glAttachShader(program, shader);
	}
	
	//glAttachShader(program, fshader);
	//if (gs) glAttachShader(program, gshader);

	glLinkProgram(program);
	Shader result = {0};
	result.gl_program = program;
	/*result.file_name = file_name;
	result.file_handle = file;*/

	return result;
}

typedef enum {
	SHADER_VERTEX		= (1<<0),
	SHADER_PIXEL		= (1<<1),
	SHADER_GEOMETRY		= (1<<2),
} ShaderType;

GLuint create_gl_shader_file(char *source, int len, GLenum type) {
	GLuint shader = glCreateShader(type);
	//int glen = strlen(source);
	char *sources[2];
	sources[1] = source;
	if (type == GL_VERTEX_SHADER) sources[0] = "#version 330\n#define SHADER_VERTEX\n";
	if (type == GL_FRAGMENT_SHADER) sources[0] = "#version 330\n#define SHADER_PIXEL\n";
	if (type == GL_GEOMETRY_SHADER) sources[0] = "#version 330\n#define SHADER_GEOMETRY\n";
	int lens[2] = {strlen(sources[0]), len};
	glShaderSource(shader, 2, sources, lens);
	glCompileShader(shader);
	int error_size;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &error_size);
	if (error_size) {
		char **error_out;
		if (type == GL_VERTEX_SHADER) error_out = &vs_error;
		if (type == GL_FRAGMENT_SHADER) error_out = &fs_error;
		if (type == GL_GEOMETRY_SHADER) error_out = &gs_error;
		*error_out = (char*)malloc(error_size);
		glGetShaderInfoLog(shader, error_size, NULL, *error_out);
		OutputDebugString(*error_out);
	}
	return shader;
}

Shader shader_from_file(char *file_name, int shader_types) {
	vs_error = NULL;
	fs_error = NULL;
	gs_error = NULL;

	HANDLE file = CreateFileA(file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FILETIME write_time;
	GetFileTime(file, NULL, NULL, &write_time);
	int file_size = GetFileSize(file, 0);
	HANDLE file_mapping = CreateFileMappingA(file, NULL, PAGE_WRITECOPY, 0, 0, 0);
	char *source = (char*)MapViewOfFileEx(file_mapping, FILE_MAP_COPY, 0, 0, 0, 0);
	GLuint program = glCreateProgram();
	if (shader_types & SHADER_VERTEX) {
		GLuint shader = create_gl_shader_file(source, file_size, GL_VERTEX_SHADER);
		if (!vs_error) glAttachShader(program, shader);
	}
	if (shader_types & SHADER_PIXEL) {
		GLuint shader = create_gl_shader_file(source, file_size, GL_FRAGMENT_SHADER);
		if (!fs_error) glAttachShader(program, shader);
	}
	if (shader_types & SHADER_GEOMETRY) {
		GLuint shader = create_gl_shader_file(source, file_size, GL_GEOMETRY_SHADER);
		if (!gs_error) glAttachShader(program, shader);
	}

	CloseHandle(file);

	if (vs_error || fs_error || gs_error) {
		Shader result = {0};
		return result;
	}

	glLinkProgram(program);
	// todo: detatch and delete shaders

	Shader result = {0};
	result.gl_program = program;
	result.file_name = file_name;
	//result.file_handle = file;
	result.last_write_time = write_time.dwLowDateTime & (write_time.dwHighDateTime<<32);
	result.types = shader_types;
	return result;
}

void use_shader(Shader *shader) {
	if (shader->file_name) {
		HANDLE file = CreateFileA(shader->file_name, /*GENERIC_READ*/0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		FILETIME write_time;
		GetFileTime(file, NULL, NULL, &write_time);
		uint64_t time = write_time.dwLowDateTime & (write_time.dwHighDateTime<<32);
		CloseHandle(file);
		if (time != shader->last_write_time) {
			glDeleteProgram(shader->gl_program);
			*shader = shader_from_file(shader->file_name, shader->types);
		}
	}
	glUseProgram(shader->gl_program);
}

void no_shader() {
	glUseProgram(0);
}