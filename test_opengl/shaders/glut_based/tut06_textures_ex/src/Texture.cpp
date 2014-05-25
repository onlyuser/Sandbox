#include <Texture.h>
#include <GL/glew.h>
#include <png.h>
#include <cstdio>
#include <string>
#include <iostream>

#define TEXTURE_LOAD_ERROR 0

namespace vt {

Texture::Texture(std::string name, const void* pixel_data, size_t width, size_t height)
    : m_name(name),
      m_skybox(false)
{
    if(pixel_data) {
        m_id = gen_texture_internal(pixel_data, width, height);
    }
}

Texture::Texture(std::string name, std::string png_filename)
    : m_name(name),
      m_skybox(false)
{
    unsigned char *pixel_data = NULL;
    size_t width  = 0;
    size_t height = 0;
    if(!read_png(png_filename, (void**)&pixel_data, &width, &height)) {
        return;
    }
    if(pixel_data) {
        m_id = gen_texture_internal(pixel_data, width, height);
        delete[] pixel_data;
    }
}

Texture::Texture(
        std::string name,
        std::string png_filename_pos_x,
        std::string png_filename_neg_x,
        std::string png_filename_pos_y,
        std::string png_filename_neg_y,
        std::string png_filename_pos_z,
        std::string png_filename_neg_z)
    : m_name(name),
      m_skybox(true)
{
    unsigned char *pixel_data_pos_x = NULL;
    unsigned char *pixel_data_neg_x = NULL;
    unsigned char *pixel_data_pos_y = NULL;
    unsigned char *pixel_data_neg_y = NULL;
    unsigned char *pixel_data_pos_z = NULL;
    unsigned char *pixel_data_neg_z = NULL;
    size_t width  = 0;
    size_t height = 0;
    if(!read_png(png_filename_pos_x, (void**)&pixel_data_pos_x, &width, &height)) {
        std::cout << "failed to load cube map positive x" << std::endl;
        return;
    }
    if(!read_png(png_filename_neg_x, (void**)&pixel_data_neg_x, &width, &height)) {
        std::cout << "failed to load cube map negative x" << std::endl;
        return;
    }
    if(!read_png(png_filename_pos_y, (void**)&pixel_data_pos_y, &width, &height)) {
        std::cout << "failed to load cube map positive y" << std::endl;
        return;
    }
    if(!read_png(png_filename_neg_y, (void**)&pixel_data_neg_y, &width, &height)) {
        std::cout << "failed to load cube map negative y" << std::endl;
        return;
    }
    if(!read_png(png_filename_pos_z, (void**)&pixel_data_pos_z, &width, &height)) {
        std::cout << "failed to load cube map positive z" << std::endl;
        return;
    }
    if(!read_png(png_filename_neg_z, (void**)&pixel_data_neg_z, &width, &height)) {
        std::cout << "failed to load cube map negative z" << std::endl;
        return;
    }
    if(
            pixel_data_pos_x &&
            pixel_data_neg_x &&
            pixel_data_pos_y &&
            pixel_data_neg_y &&
            pixel_data_pos_z &&
            pixel_data_neg_z)
    {
        m_id = gen_texture_skybox_internal(
                pixel_data_pos_x,
                pixel_data_neg_x,
                pixel_data_pos_y,
                pixel_data_neg_y,
                pixel_data_pos_z,
                pixel_data_neg_z,
                width,
                height);
        delete[] pixel_data_pos_x;
        delete[] pixel_data_neg_x;
        delete[] pixel_data_pos_y;
        delete[] pixel_data_neg_y;
        delete[] pixel_data_pos_z;
        delete[] pixel_data_neg_z;
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::bind() const
{
    if(m_skybox) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
    } else {
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
}

GLuint Texture::gen_texture_internal(const void* pixel_data, size_t width, size_t height)
{
    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
            GL_TEXTURE_2D,    // target
            0,                // level, 0 = base, no mipmap,
            GL_RGB,           // internal format
            width,            // width
            height,           // height
            0,                // border, always 0 in OpenGL ES
            GL_RGB,           // format
            GL_UNSIGNED_BYTE, // type
            pixel_data);
    return id;
}

GLuint Texture::gen_texture_skybox_internal(
        const void* pixel_data_pos_x,
        const void* pixel_data_neg_x,
        const void* pixel_data_pos_y,
        const void* pixel_data_neg_y,
        const void* pixel_data_pos_z,
        const void* pixel_data_neg_z,
        size_t width,
        size_t height)
{
    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X, // target
            0,                              // level, 0 = base, no mipmap,
            GL_RGB,                         // internal format
            width,                          // width
            height,                         // height
            0,                              // border, always 0 in OpenGL ES
            GL_RGB,                         // format
            GL_UNSIGNED_BYTE,               // type
            pixel_data_pos_x);
    glTexImage2D(
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X, // target
            0,                              // level, 0 = base, no mipmap,
            GL_RGB,                         // internal format
            width,                          // width
            height,                         // height
            0,                              // border, always 0 in OpenGL ES
            GL_RGB,                         // format
            GL_UNSIGNED_BYTE,               // type
            pixel_data_neg_x);
    glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y, // target
            0,                              // level, 0 = base, no mipmap,
            GL_RGB,                         // internal format
            width,                          // width
            height,                         // height
            0,                              // border, always 0 in OpenGL ES
            GL_RGB,                         // format
            GL_UNSIGNED_BYTE,               // type
            pixel_data_neg_y);
    glTexImage2D(
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, // target
            0,                              // level, 0 = base, no mipmap,
            GL_RGB,                         // internal format
            width,                          // width
            height,                         // height
            0,                              // border, always 0 in OpenGL ES
            GL_RGB,                         // format
            GL_UNSIGNED_BYTE,               // type
            pixel_data_pos_y);
    glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z, // target
            0,                              // level, 0 = base, no mipmap,
            GL_RGB,                         // internal format
            width,                          // width
            height,                         // height
            0,                              // border, always 0 in OpenGL ES
            GL_RGB,                         // format
            GL_UNSIGNED_BYTE,               // type
            pixel_data_pos_z);
    glTexImage2D(
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, // target
            0,                              // level, 0 = base, no mipmap,
            GL_RGB,                         // internal format
            width,                          // width
            height,                         // height
            0,                              // border, always 0 in OpenGL ES
            GL_RGB,                         // format
            GL_UNSIGNED_BYTE,               // type
            pixel_data_neg_z);
    return id;
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
    if(!fp) {
        return false;
    }

    // read the header
    fread(header, 1, 8, fp);

    // test if png
    int is_png = !png_sig_cmp(header, 0, 8);
    if(!is_png) {
        fclose(fp);
        return false;
    }

    // create png struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr) {
        fclose(fp);
        return false;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        fclose(fp);
        return false;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if(!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return false;
    }

    // png error stuff, not sure libpng man suggests this
    if(setjmp(png_jmpbuf(png_ptr))) {
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
    if(!image_data) {
        // clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return false;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep *row_pointers = new png_bytep[theight];
    if(!row_pointers) {
        //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        delete[] image_data;
        fclose(fp);
        return false;
    }
    // set the individual row_pointers to point at the correct offsets of image_data
    for(int i = 0; i < static_cast<int>(theight); ++i) {
        row_pointers[theight-1-i] = image_data+i*rowbytes;
    }

    //read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    delete[] row_pointers;
    fclose(fp);

    // clean up memory and close stuff
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

    // update width and height based on png info
    *pixel_data = image_data;
    *width      = twidth;
    *height     = theight;

    return true;
}

}
