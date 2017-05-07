
#include "rain.c"

#ifdef _WIN32
#	include <GL/gl.h>
//#	pragma comment(lib, "opengl32.lib")
#elif __APPLE__
#	include <OpenGL/gl.h>
#endif

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

typedef struct {
	int width;
	int height;
	GLuint id;
} Texture;

//GLuint load_image(char *file) {
//	GLuint tex;
//	int width;
//	int height;
//	int components = 0;
//	uint8 *image_data = stbi_load(file, &width, &height, &components, 0);
//	glGenTextures(1, &tex);
//	glBindTexture(GL_TEXTURE_2D, tex);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	stbi_image_free(image_data);
//	return tex;
//}

Sound load_audio(char *file) {
	FILE *sound = fopen(file, "rb");
	fseek(sound, 0, SEEK_END);
	int soundSize = ftell(sound);
	fseek(sound, 0, SEEK_SET);
	void *soundData = malloc(soundSize);
	fread(soundData, 1, soundSize, sound);
	Sound s = LoadSoundFromMemory(soundData, soundSize);
	fclose(sound);
	return s;
}

void draw_rect(float x, float y, float w, float h, float rotation) {
	float w12 = w/2.0f;
	float h12 = h/2.0f;
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	glRotatef(rotation/PI * 180.0f, 0.0f, 0.0f, -1.0f);
	glBegin(GL_QUADS);
		glVertex2f(-w12, -h12);
		glVertex2f(w12, -h12);
		glVertex2f(w12, h12);
		glVertex2f(-w12, h12);
	glEnd();
	glPopMatrix();
}

void draw_sprite(GLuint tex, float x, float y, float w, float h, float rotation) {
	float w12 = w/2.0f;
	float h12 = h/2.0f;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	glRotatef(rotation/PI * 180.0f, 0.0f, 0.0f, -1.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex2f(-w12, -h12);
		glTexCoord2f(1, 1); glVertex2f(w12, -h12);
		glTexCoord2f(1, 0); glVertex2f(w12, h12);
		glTexCoord2f(0, 0); glVertex2f(-w12, h12);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

Rain rain = {0};

void init();
void update();

#ifdef _WIN32
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
#elif __APPLE__
int main() {
#endif
	rain_init(&rain);
	InitSound(&rain);
	init();
	while (!rain.quit) {
		rain_update(&rain);
		UpdateSound(&rain);
		update();
	}
}

#ifdef _WIN32
void WinMainCRTStartup() {
	WinMain(0, 0, 0, 0);
	ExitProcess(0);
}
#endif
