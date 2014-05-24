#include <Texture.h>
#include <GL/glew.h>
#include <png.h>
#include <cstdio>
#include <string>
#include <iostream>

#define TEXTURE_LOAD_ERROR 0

namespace vt {

Texture::Texture(std::string name, const void* pixel_data, size_t width, size_t height)
    : m_name(name)
{
    glGenTextures(1, &m_id);
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
            GL_TEXTURE_2D,    // target
            0,                // level, 0 = base, no minimap,
            GL_RGB,           // internalformat
            width,            // width
            height,           // height
            0,                // border, always 0 in OpenGL ES
            GL_RGB,           // format
            GL_UNSIGNED_BYTE, // type
            pixel_data);
}

Texture::Texture(std::string name, std::string png_filename)
    : m_name(name)
{
    void *pixel_data = NULL;
    size_t width;
    size_t height;

    read_png(png_filename, &pixel_data, &width, &height);
    if(!pixel_data) {
        return;
    }

    // now generate the opengL m_id object
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) pixel_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    delete[] pixel_data;
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

bool Texture::read_png(std::string png_filename, void **pixel_data, size_t *width, size_t *height)
{
    if(!pixel_data || !width || !height) {
        return false;
    }

    // header for testing if it is a png
    png_byte header[8];

    // open file as binary
    FILE *fp = fopen(png_filename.c_str(), "rb");
    if (!fp) {
        return false;
    }

    // read the header
    fread(header, 1, 8, fp);

    // test if png
    int is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png) {
        fclose(fp);
        return false;
    }

    // create png struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
        NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return false;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        fclose(fp);
        return false;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return false;
    }

    // png error stuff, not sure libpng man suggests this
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return false;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 twidth, theight;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL);

    // update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // allocate the image_data as a big block, to be given to opengl
    png_byte *image_data = new png_byte[rowbytes * theight];
    if (!image_data) {
        // clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return false;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep *row_pointers = new png_bytep[theight];
    if (!row_pointers) {
        //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        delete[] image_data;
        fclose(fp);
        return false;
    }
    // set the individual row_pointers to point at the correct offsets of image_data
    for (int i = 0; i < static_cast<int>(theight); ++i)
        row_pointers[theight - 1 - i] = image_data + i * rowbytes;

    //read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    delete[] row_pointers;
    fclose(fp);

    // clean up memory and close stuff
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

    // update width and height based on png info
    *pixel_data = image_data;
    *width = twidth;
    *height = theight;

    return true;
}

}
