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
#define GL_RGB32F                         0x8815
#define GL_LINK_STATUS                    0x8B82

#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7

#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_COLOR_ATTACHMENT1              0x8CE1
#define GL_COLOR_ATTACHMENT2              0x8CE2
#define GL_COLOR_ATTACHMENT3              0x8CE3
#define GL_COLOR_ATTACHMENT4              0x8CE4
#define GL_COLOR_ATTACHMENT5              0x8CE5
#define GL_COLOR_ATTACHMENT6              0x8CE6
#define GL_COLOR_ATTACHMENT7              0x8CE7
#define GL_COLOR_ATTACHMENT8              0x8CE8
#define GL_COLOR_ATTACHMENT9              0x8CE9
#define GL_COLOR_ATTACHMENT10             0x8CEA
#define GL_COLOR_ATTACHMENT11             0x8CEB
#define GL_COLOR_ATTACHMENT12             0x8CEC
#define GL_COLOR_ATTACHMENT13             0x8CED
#define GL_COLOR_ATTACHMENT14             0x8CEE
#define GL_COLOR_ATTACHMENT15             0x8CEF
#define GL_FRAMEBUFFER                    0x8D40
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5

#define GL_RGBA32F                        0x8814
#define GL_RGB32F                         0x8815
#define GL_RGBA16F                        0x881A
#define GL_RGB16F                         0x881B

#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED        0x8CDD
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS 0x8CD9

typedef void (APIENTRY  *GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

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
	GLE(void, DeleteShader, GLuint shader)\
	GLE(void, DetachShader,	GLuint program, GLuint shader)\
	GLE(void, GetProgramiv, GLuint program, GLenum pname, GLint *params)\
	\
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
	GLE(void, Uniform1i, GLint location, int i)\
	GLE(GLint, GetAttribLocation, GLuint program, const GLchar *name)\
	\
	GLE(void, ActiveTexture, GLenum texture)\
	\
	GLE(void, GenFramebuffers, GLsizei n,  GLuint * framebuffers)\
	GLE(void, BindFramebuffer, GLenum target,  GLuint framebuffer)\
	GLE(void, FramebufferTexture, GLenum target, GLenum attachment, GLuint texture, GLint level)\
	GLE(void, FramebufferTexture2D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)\
	GLE(void, DrawBuffers, GLsizei n, const GLenum *bufs)\
	GLE(GLenum, CheckFramebufferStatus, GLenum target)\
	GLE(void, BlitFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)\
	\
	GLE(void, DebugMessageCallback, GLDEBUGPROC callback, void *userParam)\
	GLE(void, DebugMessageControl, GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled)\

#define GLE(ret, name, ...) typedef ret (GLDECL name##_proc)(__VA_ARGS__); name##_proc *gl##name;
OPENGL_EXTENSION_LIST
#undef GLE

void APIENTRY opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
	debug_print("[id:%i] %s\n", id, message);
}

void load_opengl_extensions() {
	//HMODULE opengl_lib = LoadLibraryA("opengl32.dll");

#define GLE(ret, name, ...) gl##name = (name##_proc*)wglGetProcAddress("gl" #name);
	OPENGL_EXTENSION_LIST
#undef GLE

	glDebugMessageCallback(opengl_debug_callback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
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
		OutputDebugString("----------------------------------------------------------------\n");
		OutputDebugString(*error_out);
	}
	return shader;
}

//#define gl_error() _gl_error(__FILE__, __LINE__)
//void _gl_error(char *file, int line) {
//	GLenum error;
//	while ((error = glGetError()) != GL_NO_ERROR) {
//		int x = 0;
//		debug_print("%s:%i: %s\n", file, line, gluErrorString(error));
//	}
//}

Shader shader_from_file(char *file_name, int shader_types) {
	vs_error = NULL;
	fs_error = NULL;
	gs_error = NULL;
	Shader result = {0};
	GLuint program = 0;

	FILETIME write_time = {0};
	HANDLE file = CreateFileA(file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) {
		goto end;
	}
	GetFileTime(file, NULL, NULL, &write_time);
	int file_size = GetFileSize(file, 0);
	HANDLE file_mapping = CreateFileMappingA(file, NULL, PAGE_WRITECOPY, 0, 0, 0);
	char *source = (char*)MapViewOfFileEx(file_mapping, FILE_MAP_COPY, 0, 0, 0, 0);
	program = glCreateProgram();
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

	UnmapViewOfFile(source);
	CloseHandle(file_mapping);
	CloseHandle(file);

	if (vs_error || fs_error || gs_error) {
		goto end;
	}

	glLinkProgram(program);
	// todo: detatch and delete shaders

end:
	result.gl_program = program;
	result.file_name = file_name;
	//result.file_handle = file;
	result.last_write_time = write_time.dwLowDateTime & (write_time.dwHighDateTime<<32);
	result.types = shader_types;
	return result;
}

struct FileResult {
	union {
		void *data;
		char *str;
	};
	int64 size;
};
FileResult load_file(char *file) {
	FileResult res = {};
	FILE *f = fopen(file, "rb");
	if (f) {
		fseek(f, 0, SEEK_END);
		res.size = ftell(f);
		fseek(f, 0, SEEK_SET);
		res.data = malloc(res.size);
		fread(res.data, 1, res.size, f);
		fclose(f);
	}
	return res;
}
int64 get_file_time(char *file) {
	int64 res = 0;
	HANDLE handle = CreateFileA(file, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle != INVALID_HANDLE_VALUE) {
		//FILETIME write_time;
		GetFileTime(handle, NULL, NULL, (FILETIME*)&res);
		//uint64_t time = write_time.dwLowDateTime & (write_time.dwHighDateTime<<32);
		CloseHandle(handle);
	}
	return res;
}

struct ShaderFile {
	char file[64];
	int index;
	GLenum type;
	int prog_index;
	uint64_t time;
	FileResult file_res;
	int depends[64];
	int depends_count;
};
struct ShaderProg {
	GLuint prog;
	char *vs_file;
	char *fs_file;
};

ShaderFile shader_files[64];
int shader_file_count = 0;
ShaderProg shader_progs[64];
int shader_prog_count = 0;

ShaderFile *add_shader_file(char *file) {
	for (int i = 0; i < shader_file_count; ++i) {
		if (!strcmp(shader_files[i].file, file)) return &shader_files[i];
	}
	if (shader_file_count < array_size(shader_files)) {
		ShaderFile *sf = &shader_files[shader_file_count];
		sf->index = shader_file_count++;
		return sf;
	} else {
		debug_print("Out of shader file slots!\n");
		return NULL;
	}
}

void add_shader_file_depend(ShaderFile *sf, int depend) {
	if (depend != -1) {
		for (int i = 0; i < sf->depends_count; ++i) {
			if (strcmp(shader_files[sf->depends[i]].file, shader_files[depend].file)==0) {
				return;
			}
		}
		if (sf->depends_count < array_size(sf->depends)) {
			sf->depends[sf->depends_count++] = depend;
		} else {
			debug_print("Out of depends slots for %s!\n", sf->file);
		}
	}
}

void add_shader_prog(GLuint prog, char *vs, char *fs) {
	if (shader_prog_count < array_size(shader_progs)) {
		shader_progs[shader_prog_count++] = {prog, vs, fs};
	} else {
		OutputDebugString("Out of shader prog slots!\n");
	}
}

ShaderFile *process_shader_file(char *file, int parent_file = -1) {
	// todo: maybe cache file contents so includes dont get loading multiple times
	// todo: maybe make file names all lowercase and remove extra spaces n stuff
	// todo: make sure shaders get added to the list even if they fail, so they can be reloaded eventually

	ShaderFile *sf = add_shader_file(file);
	strcpy(sf->file, file);
	sf->time = get_file_time(file);
	add_shader_file_depend(sf, parent_file);

	FileResult res = load_file(file);

	if (res.data) {
		char *mem = (char*)res.data;
		int size = res.size;
		int str = 0;
		char *inc = "#include";
		int inc_len = strlen(inc);
		for (int i = 0; i < size - inc_len; ++i) {
			int inc_start;
			if (strncmp(mem+str, inc, inc_len) == 0) {
				inc_start = str;
				str += inc_len;
				while (mem[str] != '"') ++str;
				char name[64];
				int name_len = 0;
				++str;
				while (mem[str] != '"') name[name_len++] = mem[str++];
				++str;
				name[name_len] = 0;
				//FileResult inc_file = load_file(name);
				ShaderFile *inc_file = process_shader_file(name, sf->index);
				char *newmem = (char*)realloc(mem, size + inc_file->file_res.size - inc_len);
				memcpy(newmem + inc_start + inc_file->file_res.size, newmem + str, size-str);
				memcpy(newmem + inc_start, inc_file->file_res.data, inc_file->file_res.size);
				size = inc_start + inc_file->file_res.size + (size-str);
				mem = newmem;
			} else {
				++str;
			}
		}

		debug_print("Processed shader file %s \n", file);

		mem[size] = 0;
		sf->file_res = {mem, size};
	} else {
		debug_print("Failed to load shader file %s \n", file);
	}

	return sf;
}

GLuint create_sub_shader(char *file, char *source, int size, GLenum type) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, &size);
	glCompileShader(shader);
	int error_size;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &error_size);
	if (error_size) {
		char *error = (char*)malloc(error_size);
		glGetShaderInfoLog(shader, error_size, NULL, error);
		OutputDebugString("----------------------------------------------------------------\n");
		debug_print("%s %s", file, error);
		//OutputDebugString(error);
		return 0;
	}
	return shader;
}

int create_shader_vf(char *vs, char *fs, int slot = -1) {
	vs_error = NULL;
	fs_error = NULL;
	Shader result = {0};
	GLuint program = 0;

	if (slot!=-1) {
		
	}
	program = glCreateProgram();

	int prog_index;
	if (slot == -1) {
		prog_index = shader_prog_count;
		shader_progs[shader_prog_count++] = {program, vs, fs};
	} else {
		prog_index = slot;
		glDeleteProgram(shader_progs[slot].prog);
		shader_progs[slot].prog = program;
		vs = shader_progs[slot].vs_file;
		fs = shader_progs[slot].fs_file;
	}
	/*ShaderFile vsf = {vs, GL_VERTEX_SHADER, prog_index, get_file_time(vs)};
	ShaderFile fsf = {fs, GL_FRAGMENT_SHADER, prog_index, get_file_time(fs)};*/
	ShaderFile* vsf = process_shader_file(vs);
	ShaderFile* fsf = process_shader_file(fs);

	vsf->prog_index = prog_index;
	fsf->prog_index = prog_index;
	vsf->type = GL_VERTEX_SHADER;
	fsf->type = GL_FRAGMENT_SHADER;
	if (vsf && fsf) {
		GLuint vshader = create_sub_shader(vs, vsf->file_res.str, vsf->file_res.size, GL_VERTEX_SHADER);
		glAttachShader(program, vshader);
		GLuint fshader = create_sub_shader(fs, fsf->file_res.str, fsf->file_res.size, GL_FRAGMENT_SHADER);
		glAttachShader(program, fshader);

		//if (!vshader || !fshader) return -1;

		glLinkProgram(program);
		// todo: detach and delete shaders
		glDetachShader(program, vshader);
		glDetachShader(program, fshader);
		glDeleteShader(vshader);
		glDeleteShader(fshader);

		if (slot==-1) {
			debug_print("Created shader program %i \n", prog_index);
		} else {
			debug_print("Reloaded shader program %i \n", prog_index);
		}
	} else {
		debug_print("Failed to process shader files %s %s \n", vs, fs);
	}

	return prog_index;
}

int reloads[64];
int reload_count = 0;
void add_reload(ShaderFile *sf) {
	if (reload_count < array_size(reloads)) {
		if (sf->depends_count == 0) reloads[reload_count++] = sf->prog_index;
		for (int i = 0; i < sf->depends_count; ++i) {
			add_reload(&shader_files[sf->depends[i]]);
		}
	}
}
void update_shaders() {
	for (int i = 0; i < shader_file_count; ++i) {
		int64 time = get_file_time(shader_files[i].file);
		if (time != shader_files[i].time) {
			add_reload(&shader_files[i]);
		}
	}
	for (int i = 0; i < reload_count; ++i) {
		create_shader_vf(NULL, NULL, reloads[i]);
	}
	reload_count = 0;
}

void use_shader(Shader *shader) {
	/*if (shader->file_name) {
		HANDLE file = CreateFileA(shader->file_name, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file != INVALID_HANDLE_VALUE) {
			FILETIME write_time;
			GetFileTime(file, NULL, NULL, &write_time);
			uint64_t time = write_time.dwLowDateTime & (write_time.dwHighDateTime<<32);
			CloseHandle(file);
			if (time != shader->last_write_time) {
				glDeleteProgram(shader->gl_program);
				*shader = shader_from_file(shader->file_name, shader->types);
			}
		} else {
			OutputDebugString("failed to open shader file\n");
		}
	}*/
	glUseProgram(shader->gl_program);
}

void no_shader() {
	glUseProgram(0);
}